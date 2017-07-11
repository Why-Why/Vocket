/*///////

This is the main class Vocket.
You can use this library just include this header file and get a instance of Vocket class.
I will use OO method, which will be more clearly and beautiful I think.

*////////

#ifndef VOCKET_H
#define VOCKET_H

#include "Common.h"
#include "Link/LinkController.h"

class Vocket {
	// The private variable.
	private:
		LinkController * link;

	// The public variable.
	public:

	// The private function.
	private:

	// The public function.
	public:
		Vocket(TYPE type);
		~Vocket();

		// Set the bitrate, return 1 for ok, 0 for error.
		bool SetBitrate(int bitrate);

		int Send(void * in,int len);
		int Recv(void * out,int maxlen);

		// Send to an callback function.
		int SendToFunc(void * in,int len,SEND_FUNC func);
		int RecvFromFunc(void * out,int maxlen,RECV_FUNC func);
};

#endif
