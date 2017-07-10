/*//////////////////

*///////////////////

#include <string>
#include <fstream>
#include <unistd.h>

#include "Physical/AudioPCM.h"
#include "Common.h"

using std::string;
using std::ifstream;
using std::ofstream;
using std::ios;

AudioPCM::AudioPCM(const char * name) {
	SetFileName(name);
}

AudioPCM::~AudioPCM() {
	if(inopen) in.close();
	if(outopen) out.close();
}

int AudioPCM::SetFileName(const char * name) {
	filename=string(name);
	outopen=inopen=0;
	return OK;
}

int AudioPCM::Send(DATA * data,int len) {
	if(outopen==0) out.open(filename.c_str(),ios::out | ios::trunc | ios::binary);
	outopen=1;

	if(out.is_open()==0) return ERROR_FILE;

	// Big-endian.
	for(int i=0;i<len;++i) {
		BYTE ob[2]={};
		ob[1]=BYTE(data[i] & 0xff);
		ob[0]=BYTE((data[i]>>8) & 0xff);

		out.write((char *)ob,2);
	}

	sleep(len/(double)DEF_RATE);		// !!!!

	return len;
}

int AudioPCM::Recv(DATA * data,int maxlen) {
	if(inopen==0) in.open(filename.c_str(),ios::in | ios::binary);
	inopen=1;
	if(in.is_open()==0) return ERROR_FILE;

	DATA a,b;
	int len=0;

	for(int i=0;i<maxlen;++i) {
		if(in.eof()) break;
		b=in.get();
		if(in.eof()) break;
		a=in.get();

		// Big-endian.
		data[len++]=(a & 0xff) | ((int(b))<<8);
	}

	return len;
}
