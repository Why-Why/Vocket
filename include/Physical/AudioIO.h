/*////////////////////

This is the Audio IO class.
Which will be an interface for different instance.

This will contain the control of audio, will include the input and output of voice file through file or sound and mic.

*/////////////////////

#ifndef AUDIOIO_H
#define AUDIOIO_H

#include "Common.h"

class AudioIO {
	private:

	public:
		virtual int Send(DATA * data,int len)=0;
		virtual int Recv(DATA * data,int maxlen)=0;

		int SendToFunc(DATA * data,int len,SEND_FUNC func);
		int RecvFromFunc(DATA * data,int maxlen,RECV_FUNC func);
};

#endif
