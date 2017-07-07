/*//////////////////

*///////////////////

#include "Common.h"
#include "Physical/ModulatorFSK.h"

#include <cstdlib>
#include <cmath>
#include <algorithm>

using std::max;
using std::min;
using std::sin;

ModulatorFSK::ModulatorFSK() {
	Set(DEF_RATE,DEF_RATE0,DEF_RATE1,DEF_WINDOWLEN,DEF_AMPLITUDE);
}

ModulatorFSK::ModulatorFSK(int Rate,int BitRate) {
	int windowlen=Rate/BitRate;
	int rate0=windowlen;
	int rate1=rate0/2;

	Set(Rate,rate0,rate1,windowlen,DEF_AMPLITUDE);
}

ModulatorFSK::ModulatorFSK(int Rate,int Rate0,int Rate1,int WindowLen,int Amplitude) {
	Set(Rate,Rate0,Rate1,WindowLen,Amplitude);
}

int ModulatorFSK::Set(int rate,int rate0,int rate1,int windowlen,int amplitude) {
	if(Check(rate,rate0,rate1,windowlen,amplitude)!=OK) return ERROR_PARAMETERS;

	Rate=rate;
	Rate0=rate0;
	Rate1=rate1;
	WindowLen=windowlen;
	Amplitude=amplitude;

	return OK;
}

int ModulatorFSK::SetBitrate(int bitrate) {
	WindowLen=(Rate+bitrate-1)/bitrate;
	return OK;
}

int ModulatorFSK::Check(int rate,int rate0,int rate1,int windowlen,int amplitude) {
	if(!(windowlen>=max(rate0,rate1))) return NO;
	if(!(min(rate0,rate1)>=2)) return NO;
	return OK;
}

// Math Place.
int ModulatorFSK::Encode(BIT bit,DATA * out) {
	int rateneed;

	switch(bit) {
		case 0:
			rateneed=Rate0;
			break;
		case 1:
			rateneed=Rate1;
			break;
		default:
			return ERROR_PARAMETERS;
	}

	///////////////

	// The function is A*sin(kx);
	double k=2*PI/rateneed;
	double A=Amplitude;

	for(int i=0;i<WindowLen;++i) out[i]=A*cos(k*i);

	return WindowLen;
}

int ModulatorFSK::Encode(BIT * data,int len,DATA * out) {
	if(Check(Rate,Rate0,Rate1,WindowLen,Amplitude)!=OK) return ERROR_PARAMETERS;

	int ret=0,tmp;

	for(int i=0;i<len;++i) {
		tmp=Encode(data[i],out+ret);
		ret+=tmp;
	}

	return ret;
}

int ModulatorFSK::GetEncodeLen(int len) {
	return WindowLen*len;
}
