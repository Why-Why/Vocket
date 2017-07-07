/*///////////////

Demodulator, which will decoder the voice data recived.

This is an interface.

*////////////////

#ifndef DEMODULATOR_H
#define DEMODULATOR_H

#include "Common.h"

class Demodulator {
	private:

	public:
		virtual int Data2Sig(DATA * in,int len,int * sig)=0;
		virtual int FindStr(int * sig,int len)=0;
		virtual int Decode(int * sig,int len,BIT * out,int str,int & nextstr)=0;

		virtual int GetDataLen(int siglen)=0;
		virtual int GetSigLen(int datalen)=0;
		virtual int GetBitLen(int siglen)=0;
		virtual int GetLastdataLen()=0;		// Get the length of data that will be retained for the next time to decode. (used for PhyController Recv).

		virtual int SetBitrate(int bitrate)=0;
};

#endif
