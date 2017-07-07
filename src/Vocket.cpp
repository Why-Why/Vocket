/*//////////////////////

*///////////////////////

#include "Common.h"
#include "Vocket.h"

Vocket::Vocket(TYPE type) {
	link=new LinkController(type);
}

Vocket::~Vocket() {
	if(link) delete link;
}

bool Vocket::SetBitrate(int bitrate) {
	if(link==0) return 0;
	if(link->SetBitrate(bitrate)==OK) return 1;
	return 0;
}

int Vocket::Send(void * in,int len) {
	if(link==0) return NO;
	return link->Send((BYTE *)in,len);
}

int Vocket::Recv(void * out,int maxlen) {
	if(link==0) return NO;
	return link->Recv((BYTE *)out,maxlen);
}
