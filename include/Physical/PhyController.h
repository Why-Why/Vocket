/*//////////////////////

This is the controller of physical layer.
Which will come true:
1. the package of data from link layer, which will add an header for synchronization, and an ender.
2. the modulator and the demodulator, which will encode and decode the binary bits to voice data (PCM data). I will use FSK and PSK even MSK and GMSK etc.
3. the controller of audio, which will output the voice data through system audio library, or through the wav/pcm file library.

*///////////////////////

#ifndef PHYCONTROLLER_H
#define PHYCONTROLLER_H

#include "Common.h";
#include "Physical/AudioIO.h";

class PhyController {
	private:
		const static MODULATOR_TYPE def_modula=MODULATOR_FSK;
		const static DEMODULATOR_TYPE def_demodula=DEMODULA_FSK_DSTFT;
		const static AUDIO_TYPE def_audio=AUDIO_PCM;

	public:

	private:

	public:
		PhyController();
		// Chose which modulator and demodulator and voice input and voice outpu.
		PhyController(MODULATOR_TYPE modula,DEMODULATOR_TYPE dumo,AUDIO_TYPE input,AUDIO_TYPE output);

		// Send the data, whose length is len.
		// return the length of data that is successful to be sended, -1 for error.
		int Send(BYTE * data,int len);

		// Recv the data, save in the data, whill get at most maxlen length of data.
		// return the actual length of data, -1 for error.
		int Recv(BYTE * data,int maxlen);
};

#endif
