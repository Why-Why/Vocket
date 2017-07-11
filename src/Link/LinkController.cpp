/*/////////////////////

*//////////////////////

#include "Common.h"
#include "Link/LinkController.h"
#include "Link/LinkPackage.h"

#include <unistd.h>

LinkController::LinkController(TYPE type) {
	phy=0;
	package=0;

	IOType=type;

	switch(type) {
		case VF_VOICE:
			phy=new PhyController(MODULATOR_FSK,DEMODULATOR_FSK_DSTFT,AUDIO_ALSA,AUDIO_ALSA);
			break;
		case VF_FILE:
			phy=new PhyController(MODULATOR_FSK,DEMODULATOR_FSK_DSTFT,AUDIO_PCM,AUDIO_PCM);
			break;
		default:
			// Error
			break;
	}

	package=new LinkPackage();
}

LinkController::~LinkController() {
	if(phy) delete phy;
	if(package) delete package;
}

int LinkController::SetBitrate(int bitrate) {
	if(phy==0) return NO;
	return phy->SetBitrate(bitrate);
}

int LinkController::Send(BYTE * in,int len) {
	if(package==0 || phy==0) return NO;

	Packet * packet=new Packet [package->GetPackageLen(len)];
	int packlen=package->Package(in,len,packet);
	if(packlen<0) return packlen;

	int ret=0;
	for(int i=0;i<packlen;++i) ret+=Send(packet[i]);

	delete [] packet;

	return ret;
}

int LinkController::Send(Packet & pack) {
	int ret=phy->Send((BYTE *)(&pack),package->GetByteLen());

	if(IOType==VF_VOICE) sleep(DEF_RECVDATALEN/(double)DEF_RATE);		// !!!
	else if(IOType==VF_FILE) {
	}

	return ret;
}

int LinkController::Recv(BYTE * out,int maxlen) {
	if(phy==0 || package==0) return NO;

	BYTE * recvtmp=new BYTE [package->GetByteLen()];
	BYTE * bytetmp=new BYTE [package->GetByteLen()];
	int ret=0;

	while(1) {
		int len=phy->Recv(recvtmp,10);		// !!!

		if(len!=10) {
			// ERROR!!!
			delete [] recvtmp;
			delete [] bytetmp;
			return NO;
		}

		bool isfinal;
		len=package->UnPackage((Packet *)recvtmp,bytetmp,isfinal);

		if(len<0) {
			delete [] recvtmp;
			delete [] bytetmp;
			return len;
		}

		for(int i=0;i<len;++i) out[ret++]=bytetmp[i];

		if(isfinal) break;
	}

	delete [] recvtmp;
	delete [] bytetmp;

	return ret;
}

int LinkController::SendToFunc(BYTE * in,int len,SEND_FUNC func) {
	if(package==0 || phy==0) return NO;

	Packet * packet=new Packet [package->GetPackageLen(len)];
	int packlen=package->Package(in,len,packet);
	if(packlen<0) return packlen;

	int ret=0;
	for(int i=0;i<packlen;++i) ret+=SendToFunc(packet[i],func);

	delete [] packet;

	return ret;
}

int LinkController::SendToFunc(Packet & pack,SEND_FUNC func) {
	int ret=phy->SendToFunc((BYTE *)(&pack),package->GetByteLen(),func);
	return ret;
}

int LinkController::RecvFromFunc(BYTE * out,int maxlen,RECV_FUNC func) {
	if(phy==0 || package==0) return NO;

	BYTE * recvtmp=new BYTE [package->GetByteLen()];
	BYTE * bytetmp=new BYTE [package->GetByteLen()];
	int ret=0;

	while(1) {
		int len=phy->RecvFromFunc(recvtmp,10,func);		// !!!

		if(len!=10) {
			// ERROR!!!
			delete [] recvtmp;
			delete [] bytetmp;
			return NO;
		}

		bool isfinal;
		len=package->UnPackage((Packet *)recvtmp,bytetmp,isfinal);

		if(len<0) {
			delete [] recvtmp;
			delete [] bytetmp;
			return len;
		}

		for(int i=0;i<len;++i) out[ret++]=bytetmp[i];

		if(isfinal) break;
	}

	delete [] recvtmp;
	delete [] bytetmp;

	return ret;
}
