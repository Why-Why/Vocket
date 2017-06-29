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
		virtual int Decode(DATA * in,int len,BIT * out)=0;
};

#endif
