/*////////////////

Use ALSA library to come true voice play and record.

*/////////////////

#ifndef AUDIOALSA_H
#define AUDIOALSA_H

#include <alsa/asoundlib.h>

#include "Common.h"
#include "Physical/AudioIO.h"

class AudioALSA : public AudioIO {
	private:
		bool inopen,outopen;
		snd_pcm_t * handle_in,* handle_out;
		snd_pcm_uframes_t frames_in,frames_out;

	public:

	private:
		void InitOut();
		void InitIn();

	public:
		AudioALSA();
		~AudioALSA();

		int Send(DATA * data,int len);
		int Recv(DATA * data,int maxlen);
};

#endif
