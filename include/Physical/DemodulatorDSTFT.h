/*///////////////////

This is an instance of Demodulator, use DSTFT methods the decode FSK data.

*////////////////////

#include "Physical/Demodulator.h"

class DemodulatorDSTFT : public Demodulator {
	private:
		int WindowLen;

	public:

	private:

	public:
		DemodulatorDSTFT();
		DemodulatorDSTFT(int WindowLen);

		int SetWindowLen(int WindowLen);

		int Decode(DATA * in,int len,BIT * out);
};
