/*///////////////

*////////////////

#include "Common.h"
#include "Physical/AudioIO.h"

int AudioIO::SendToFunc(DATA * data,int len,SEND_FUNC func) {
	return func(data,len);
}

int AudioIO::RecvFromFunc(DATA * data,int len,RECV_FUNC func) {
	return func(data,len);
}
