/*///////////////////

This is an instance of Demodulator, use DSTFT methods the decode FSK data.

*////////////////////

#include "Physical/Demodulator.h"

class DemodulatorDSTFT : public Demodulator {
	private:
		int Rate,Rate0,Rate1,WindowLen;

	public:

	private:

	public:
		DemodulatorDSTFT();
		DemodulatorDSTFT(int Rate,int Rate0,int Rate1,int WindowLen);

		int Set(int Rate,int Rate0,int Rate1,int WindowLen);

		int Decode(DATA * in,int len,BIT * out);
};
