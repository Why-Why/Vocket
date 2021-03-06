/*/////////////

This is some common data for used.

*//////////////

#ifndef COMMON_H
#define COMMON_H

// Use which method to communicate, voice or file.
enum TYPE {
	VF_VOICE=1,
	VF_FILE=2
};

enum MODULATOR_TYPE {
	MODULATOR_FSK=1,
	MODULATOR_PSK=2
};

enum DEMODULATOR_TYPE {
	DEMODULATOR_FSK_DSTFT=11,	// The DSTFT method for FSK the decoder.
	DEMODULATOR_FSK_NONCOHERENT=12	// The noncoherent method for FSK to decode.
};

enum AUDIO_TYPE {
	AUDIO_PCM=1,	// through pcm file.
	AUDIO_ALSA=2,	// through alsa library.
};

typedef unsigned char BYTE;	// for data to be output.
typedef short DATA;	// for the music data (16 bits PCM).
typedef bool BIT;

typedef int (* SEND_FUNC) (DATA *,int);
typedef int (* RECV_FUNC) (DATA *,int);

const double PI=3.14159265358979323846;

const int OK=0;
const int NO=-0x3f3f3f3f;
const int ERROR_FILE=-1;
const int ERROR_PARAMETERS=-2;
const int ERROR_NOSTART=-3;
const int ERROR_NOEND=-4;

const int ERROR_NOINPUT=-5;
const int ERROR_NOOUTPUT=-6;
const int ERROR_NOMODULATOR=-7;
const int ERROR_NODEMODULATOR=-8;
const int ERROR_NOPACKAGE=-9;
const int ERROR_ERRORDATA=-10;

// For Modulator and Demodulator, default data.
const int DEF_RATE=44100;
const int DEF_RATE0=2;
const int DEF_RATE1=4;
const int DEF_WINDOWLEN=4410;
const int DEF_AMPLITUDE=(1<<15)-1;

const int DEF_GOELEN=8;	// The len of Goertzel algorithm, through test get this val.
const int DEF_AVELEN=128;

const int DEF_SIG_THRES=1000000;	// The minimum val a signal should have.
const int DEF_SIGMUL_THRES=2;	// A/B>DEF_SIGNUL_THRES will be treated as different.
const double DEF_HOLD_THRES=0.75;

const int DEF_STRFLAGLEN=8;
const int DEF_ENDFLAGLEN=8;
const int DEF_STRFLAG[]={0,1,0,1,0,1,0,0};
const int DEF_ENDFLAG[]={0,1,0,1,0,1,1,1};
const int DEF_FLAG[]={0,1,0,1,0,1,0,1};		// Escape character.
const BYTE DEF_STRFLAG_BYTE=84;
const BYTE DEF_ENDFLAG_BYTE=87;
const BYTE DEF_FLAG_BYTE=85;

const char DEF_INPUTFILENAME[]="input.pcm";
const char DEF_OUTPUTFILENAME[]="output.pcm";
const int DEF_RECVDATALEN=44100;	// The length AudioIO input will get once.

// For Link Layer.
const int DEF_LINKPACKAGE=8;	// Only change there is not useful.

#endif
