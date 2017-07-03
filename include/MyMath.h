/*//////////////////

This is some math algorithm.

*///////////////////

#include "Common.h"

// Goertzel algorithm, See Wiki for more info.
double Goertzel(DATA * data,int len,int Rate0);

// Get the Spectral density of a wave, through Goertzel algorithm, return the length of out.
int GetSpectral(DATA * data,int len,double * out,int Rate0,int GoeLen=DEF_GOELEN);

// Get the average of a wave, Avelen be the half of WindowLen will be better.
int GetAverage(double * data,int len,double * out,int AveLen=DEF_AVELEN);

// Get the signal of a wave.
// out[i]: -1 is no signal, 0 is 0 signal, 1 is 1 signal.
int GetSig(double * data0,int len0,double * data1,int len1,int * out);
