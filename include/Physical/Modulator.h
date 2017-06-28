/*/////////////////

Modulator, input the 01 bits data, get an voice data.
Which will be an long array, each item is an value, indicates the value of voice, and so the array is ans acoustic wave.

This is an interface.

*//////////////////

#ifndef MODULATOR_H
#define MODULATOR_H

#include "Common.h"

class Modulator {
	private:

	public:
		virtual int Encode(BIT * data,int len,DATA * out)=0;
};

#endif
