/*/////////////////

This is an modulator uses FSK.

*//////////////////

#ifndef MODULATORFSK_H
#define MODULATORFSK_H

#include "Common.h"
#include "Physical/Modulator.h"

class ModulatorFSK : public Modulator {
	private:

	public:
		ModulatorFSK();
		ModulatorFSK(

		int Encode(BIT * data,int len);
};

#endif
