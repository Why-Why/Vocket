/*/////////////////

*//////////////////

#include "Physical/PhyController.h"

#include "Physical/ModulatorFSK.h"
#include "Physical/DemodulatorDSTFT.h"
#include "Physical/AudioPCM.h"
#include "Physical/PhyPackage.h"

PhyController::PhyController() {
	PhyController(def_modula,def_demodula,def_audio,def_audio);
};

PhyController::PhyController(MODULATOR_TYPE modula,DEMODULATOR_TYPE demo,AUDIO_TYPE intype,AUDIO_TYPE outtype) {
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
			input=new AudioPCM(DEF_FILENAME);
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
			output=new AudioPCM(DEF_FILENAME);
			break;
		case AUDIO_ALSA:
			output=0;
			break;
		default:
			output=0;
			break;
	}

	package=new PhyPackage();
};

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

int PhyController::Recv(BYTE * data,int maxlen) {
	if(!package) return ERROR_NOPACKAGE;
	if(!demodulator) return ERROR_NODEMODULATOR;
	if(!input) return ERROR_NOINPUT;

	int ret=OK;
	int buflen=maxlen*2*8;

	DATA * data=new DATA [
}
