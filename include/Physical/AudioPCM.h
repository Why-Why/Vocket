/*///////////////

This is an instance of AudioIO interface.
That will input or output the voice throught a PCM file.
The code method of voice is 16 bits, 44100 Hz, 1 channel.

Note: Each Object of AudioPCM class can be used for only one place, if you want to input and output you need two object.

*////////////////

#ifndef AUDIOPCM_H
#define AUDIOPCM_H

#include <string>

#include "Common.h"
#include "AudioIO.h"

class AudioPCM : public AudioIO {
	private:
		// The file which will be used to input or output.
		// The file will be opened and closed each time of Send or Recv.
		string filename;

	public:
		AudioPCM(const char * filename);
		int SetFileName(const char * filename);

		int Send(DATA * data,int len);
		int Recv(DATA * data,int maxlen);
};

#endif
