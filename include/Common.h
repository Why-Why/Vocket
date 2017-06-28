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
	DEMODULA_FSK_DSTFT=11,	// The DSTFT method for FSK the decoder.
	DEMODULA_FSK_NONCOHERENT=12	// The noncoherent method for FSK to decode.
};

enum AUDIO_TYPE {
	AUDIO_PCM=1,	// through pcm file.
	AUDIO_ALSA=2,	// through alsa library.
	AUDIO_WAV=3	// through wav file (sndfile library).
};

typedef unsigned char BYTE;	// for data to be output.
typedef short DATA;	// for the music data (16 bits PCM).
typedef bool BIT;

const double PI=3.14159265358979323846;

const int OK=0;
const int NO=-0x3f3f3f3f;
const int ERROR_FILE=-1;
const int ERROR_PARAMETERS=-2;

// For Modulator, default data.
const int DEF_RATE=44100;
const int DEF_RATE0=128;
const int DEF_RATE1=64;
const int DEF_WINDOWLEN=128;
const int DEF_AMPLITUDE=(1<<15)-1;

#endif
