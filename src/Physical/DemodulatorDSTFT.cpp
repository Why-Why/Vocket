/*/////////////////

DSTFT mathod for decode.

*//////////////////

#include "Common.h"
#include "Physical/DemodulatorDSTFT.h"

DemodulatorDSTFT::DemodulatorDSTFT() {
	SetWindowLen(DEF_WINDOWLEN);
}

DemodulatorDSTFT::DemodulatorDSTFT(int windowlen) {
	SetWindowLen(windowlen);
}

int DemodulatorDSTFT::SetWindowLen(int windowlen) {
	WindowLen=windowlen;
	return OK;
}

int DemodulatorDSTFT::Decode(DATA * in,int len,BIT * out) {

}
