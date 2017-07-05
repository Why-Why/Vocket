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
};

#endif
