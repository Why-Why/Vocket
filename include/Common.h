/*/////////////

This is some common data for used.

*//////////////

#ifndef COMMON_H
#define COMMON_H

enum MODULATOR_TYPE {
	MODULATOR_FSK=1,
	MODULATOR_PSK=2,
	MODULATOR_MSK=3,
	MODULATOR_GMSK=4
};

enum DEMODULATOR_TYPE {
	DEMODULA_FSK_DSTFT=11;	// The DSTFT method for FSK the decoder.
	DEMODULA_FSK_NONCOHERENT=12;	// The noncoherent method for FSK to decode.
};

enum AUDIO_TYPE {
	AUDIO_PCM=1;	// through pcm file.
	AUDIO_ALSA=2;	// through alsa library.
	AUDIO_WAV=3;	// through wav file (sndfile library).
};

typedef unsigned char BYTE;	// for data to be output.
typedef short DATA;	// for the music data (16 bits PCM).

const int OK=0;
const int FILE_ERROR=-1;

#endif
