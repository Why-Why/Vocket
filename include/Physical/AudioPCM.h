/*///////////////

This is an instance of AudioIO interface.
That will input or output the voice throught a PCM file.
The code method of voice is 16 bits, 44100 Hz, 1 channel.

Note: Each Object of AudioPCM class can be used for only one place, if you want to input and output you need two object.

*////////////////

#ifndef AUDIOPCM_H
#define AUDIOPCM_H

#include <string>
#include <fstream>

#include "Common.h"
#include "Physical/AudioIO.h"

using std::string;
using std::ifstream;
using std::ofstream;
using std::ios;

class AudioPCM : public AudioIO {
	private:
		// The file which will be used to input or output.
		// The file will be opened and closed each time of Send or Recv.
		std::string filename;
		bool inopen,outopen;
		ofstream out;
		ifstream in;

	public:
		AudioPCM(const char * filename);
		~AudioPCM();
		int SetFileName(const char * filename);

		int Send(DATA * data,int len);
		int Recv(DATA * data,int maxlen);
};

#endif
