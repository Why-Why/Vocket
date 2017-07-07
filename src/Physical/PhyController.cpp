/*/////////////////

*//////////////////

#include "Physical/PhyController.h"

#include "Physical/ModulatorFSK.h"
#include "Physical/DemodulatorDSTFT.h"
#include "Physical/AudioPCM.h"
#include "Physical/PhyPackage.h"

PhyController::PhyController() {
	Set(def_modula,def_demodula,def_audio,def_audio);
};

PhyController::PhyController(MODULATOR_TYPE modula,DEMODULATOR_TYPE demo,AUDIO_TYPE intype,AUDIO_TYPE outtype) {
	Set(modula,demo,intype,outtype);
};

void PhyController::Set(MODULATOR_TYPE modula,DEMODULATOR_TYPE demo,AUDIO_TYPE intype,AUDIO_TYPE outtype) {
	switch(modula) {
		case MODULATOR_FSK:
			modulator=new ModulatorFSK();
			break;
		case MODULATOR_PSK:
			modulator=0;
			break;
		default:
			modulator=0;
			break;
	};

	switch(demo) {
		case DEMODULATOR_FSK_DSTFT:
			demodulator=new DemodulatorDSTFT();
			break;
		case DEMODULATOR_FSK_NONCOHERENT:
			demodulator=0;
			break;
		default:
			demodulator=0;
			break;
	}

	switch(intype) {
		case AUDIO_PCM:
			input=new AudioPCM(DEF_INPUTFILENAME);
			break;
		case AUDIO_ALSA:
			input=0;
			break;
		default:
			input=0;
			break;
	}

	switch(outtype) {
		case AUDIO_PCM:
			output=new AudioPCM(DEF_OUTPUTFILENAME);
			break;
		case AUDIO_ALSA:
			output=0;
			break;
		default:
			output=0;
			break;
	}

	package=new PhyPackage();
}

PhyController::~PhyController() {
	if(input) delete input;
	if(output) delete output;
	if(modulator) delete modulator;
	if(demodulator) delete demodulator;
	if(package) delete package;
}

AudioIO * PhyController::GetInput() {
	return input;
}

AudioIO * PhyController::GetOutput() {
	return output;
}

Modulator * PhyController::GetModulator() {
	return modulator;
}

Demodulator * PhyController::GetDemodulator() {
	return demodulator;
}

int PhyController::Send(BYTE * byte,int bytelen) {
	if(!package) return ERROR_NOPACKAGE;
	if(!modulator) return ERROR_NOMODULATOR;
	if(!output) return ERROR_NOOUTPUT;

	int ret=OK;
	BIT * bit=0;
	DATA * data=0;
	int tmp,bitlen,datalen;

	bit=new BIT[package->GetPackageLen(bytelen)];
	bitlen=package->Package(byte,bytelen,bit);
	if(bitlen<0) {
		ret=bitlen;
		goto finish;
	}

	data=new DATA[modulator->GetEncodeLen(bitlen)];
	datalen=modulator->Encode(bit,bitlen,data);

	if(datalen<0) {
		ret=datalen;
		goto finish;
	}

	tmp=output->Send(data,datalen);
	if(tmp<0) ret=tmp;

finish:

	if(bit) delete [] bit;
	if(data) delete [] data;

	return ret;
}

// maxdatalen is the max length AudioIO input will read one times.
int PhyController::RecvSig(int * sig,int maxdatalen) {
	DATA * data=new DATA [maxdatalen];
	int datalen=input->Recv(data,maxdatalen);
	if(datalen<=0) return 0;

	int siglen=demodulator->Data2Sig(data,datalen,sig);

	delete [] data;

	return siglen;
}

int PhyController::Recv(BYTE * byte,int maxlen) {
	if(!package) return ERROR_NOPACKAGE;
	if(!demodulator) return ERROR_NODEMODULATOR;
	if(!input) return ERROR_NOINPUT;

/*
	int tmp;

	int readdatalen=DEF_RECVDATALEN;		// Each time read readdatalen length of data from AudioIO input.
	int lastdatalen=demodulator->GetLastdataLen();
	int datalen;
	DATA * data=new DATA [readdatalen+lastdatalen];

	for(int i=0;i<lastdatalen;++i) data[i]=0;
	datalen=input->Recv(data+lastdatalen,readdatalen);
	if(datalen<=0) goto finish;
	datalen+=lastdatalen;

	while(1) {
		int * sig=new int [demodulator->GetSigLen(datalen)];
		int siglen=demodulator->Data2Sig(data,datalen,sig);
		int str=demodulator->FindStr(sig,siglen);

		if(str<0) {
			for(int i=datalen-lastdatalen;i<datalen;++i) data[i-datalen+lastdatalen]=data[i];
			datalen=input->Recv(data+lastdatalen,readdatalen);
			if(datalen<=0) {
				delete [] sig;	// Not beautiful.
				break;
			}
			datalen+=lastdatalen;
		}
		else {

		}

		delete [] sig;
	}

	delete [] data;
*/

	int maxdatalen=DEF_RECVDATALEN;
	int maxsiglen=demodulator->GetSigLen(maxdatalen);
	int * sig=new int [maxsiglen*2];

	const int maxbitlen=maxlen*8;
	BIT * bit=new BIT [maxbitlen];
	int bitlen=0;

	int siglen=RecvSig(sig,maxdatalen*2);

	while(1) {
		int str=demodulator->FindStr(sig,siglen);

		if(str<0) {
			if(siglen>=maxsiglen) {
				for(int i=siglen-maxsiglen;i<siglen;++i) sig[i-siglen+maxsiglen]=sig[i];
				siglen=maxsiglen;
			}

			int tmp=RecvSig(sig+siglen,maxdatalen);
			if(tmp<=0) break;
			siglen+=tmp;
		}
		else {
			bitlen=0;

			while(1) {
				int nextstr;
				BIT * tmpbit=new BIT [demodulator->GetBitLen(siglen)];
				int tmplen=demodulator->Decode(sig,siglen,tmpbit,str,nextstr);

				for(int i=0;i<tmplen;++i) {
					bit[bitlen++]=tmpbit[i];
					if(bitlen>=maxbitlen) break;
				}
				delete [] tmpbit;

				if(nextstr<0 || bitlen>=maxbitlen) break;
				if(nextstr==0) {
					// NO !!!
				}

				for(int i=nextstr;i<siglen;++i) sig[i-nextstr]=sig[i];
				siglen=siglen-nextstr;

				int tmp=RecvSig(sig+siglen,demodulator->GetDataLen(2*maxsiglen-siglen));
				if(tmp<=0) break;
				siglen+=tmp;

				str=0;
			}
			break;
		}
	}

	delete [] sig;

	///////////////
	// Have got bits.

	int ret=package->UnPackage(bit,bitlen,byte);
	delete [] bit;

	return ret;
}

int PhyController::SetBitrate(int bitrate) {
	if(demodulator==0) return ERROR_NODEMODULATOR;
	if(modulator==0) return ERROR_NOMODULATOR;

	demodulator->SetBitrate(bitrate);
	modulator->SetBitrate(bitrate);

	return OK;
}
