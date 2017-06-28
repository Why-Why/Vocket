/*//////////////////

*///////////////////

#include <string>
#include <fstream>

#include "Physical/AudioPCM.h"
#include "Common.h"

using std::string;
using std::ifstream;
using std::ofstream;
using std::ios;

AudioPCM::AudioPCM(const char * name) {
	SetFileName(name);
}

int AudioPCM::SetFileName(const char * name) {
	filename=string(name);
	return OK;
}

int AudioPCM::Send(DATA * data,int len) {
	ofstream out(filename.c_str(),ios::out | ios::trunc | ios::binary);
	if(out.is_open()==0) return ERROR_FILE;

	// Big-endian.
	for(int i=0;i<len;++i) {
		BYTE ob[2]={};
		ob[1]=BYTE(data[i] & 0xff);
		ob[0]=BYTE((data[i]>>8) & 0xff);

		out.write((char *)ob,2);
	}

	out.close();

	return len;
}

int AudioPCM::Recv(DATA * data,int maxlen) {
	ifstream in(filename.c_str(),ios::in | ios::binary);
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
