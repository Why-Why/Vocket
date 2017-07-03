/*///////////////////

This is an instance of Demodulator, use DSTFT methods the decode FSK data.

*////////////////////

#include "Physical/Demodulator.h"

class DemodulatorDSTFT : public Demodulator {
	private:
		int Rate,Rate0,Rate1,WindowLen;

	public:

	private:
		// Check whether there is a long time for a signal (instead of noise), get the left and right of this signal.
		bool CheckHold(int * sig,int len,int pos,int & L,int & R);

		// Check wheter that is a start flag, (01010100).
		int CheckStr(int * sig,int len,int pos);

		// Find the position of start, -1 for not find.
		int FindStr(int * sig,int len);

		// Get the 01 final bits.
		int Decode(int * sig,int len,BIT * out,int str);

	public:
		DemodulatorDSTFT();
		DemodulatorDSTFT(int Rate,int Rate0,int Rate1,int WindowLen);

		int Set(int Rate,int Rate0,int Rate1,int WindowLen);

		int Decode(DATA * in,int len,BIT * out);
};
