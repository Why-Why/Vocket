/*/////////////////

DSTFT mathod for decode.

*//////////////////

#include "Common.h"
#include "Physical/DemodulatorDSTFT.h"

DemodulatorDSTFT::DemodulatorDSTFT() {
	Set(DEF_RATE,DEF_RATE0,DEF_RATE1,DEF_WINDOWLEN);
}

DemodulatorDSTFT::DemodulatorDSTFT(int Rate,int Rate0,int Rate1,int WindowLen) {
	Set(Rate,Rate0,Rate1,WindowLen);
}

int DemodulatorDSTFT::Set(int rate,int rate0,int rate1,int windowlen) {
	Rate=rate;
	Rate0=rate0;
	Rate1=rate1;
	WindowLen=windowlen;
	return OK;
}

///////////////////////////////

int DemodulatorDSTFT::Decode(DATA * in,int len,BIT * out) {
	int step=WindowLen/8;
}
