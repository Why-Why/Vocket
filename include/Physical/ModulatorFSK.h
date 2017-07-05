/*/////////////////

This is an modulator uses FSK.

For the parameter of class ModulatorFSK, there are some constraints:
WindowLen>=max(Rate0,Rate1);
min(Rate0,Rate1)>=4;

*//////////////////

#ifndef MODULATORFSK_H
#define MODULATORFSK_H

#include "Common.h"
#include "Physical/Modulator.h"

class ModulatorFSK : public Modulator {
	private:
		int Rate,Rate0,Rate1,WindowLen,Amplitude;
		// The function is sin(2*PI*x/Rate0)*Amplitude.

	public:

	private:
		// Check whether Rate,Rate0,Rate1,WindowLen is legal.
		// See the top of this file for more info.
		int Check(int Rate,int Rate0,int Rate1,int WindowLen,int Amplitude);

		// Math place !!!
		int Encode(BIT bit,DATA * out);

	public:
		ModulatorFSK();	// Default FSK setting.
		// Set the rate of music (44100Hz etc), set the bit rate of this wave, that means we will have an window whos length is Rate/BitRate \
		// And have an Rate0 of Rate/BitRate, Rate1 is 2*Rate/BitRate.
		ModulatorFSK(int Rate,int BitRate);
		// Set the rate of music, the rate of 0 sample, the rate of 1 sample, the length of window.
		// The Rate is the number of nodes per second, Rate0/Rate1 is the number of nodes per period, WindowLen is the number of nodes per sample.
		// You can see more through the ./doc/wave.png.
		ModulatorFSK(int Rate,int Rate0,int Rate1,int WindowLen,int Amplitude);

		int Set(int Rate,int Rate0,int Rate1,int WindowLen,int Amplitude);

		// The length of out should >= len*WindowLen;
		int Encode(BIT * data,int len,DATA * out);

		int GetEncodeLen(int len);
};

#endif
