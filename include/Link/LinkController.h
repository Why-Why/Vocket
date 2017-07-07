/*/////////////////

The Controller of Link Layer.
Will be used by Vocket, and used the Physical layer controller.

*//////////////////

#ifndef LINKCONTROLLER_H
#define LINKCONTROLLER_H

#include "Common.h"
#include "Link/LinkPackage.h"
#include "Physical/PhyController.h"

class LinkController {
	private:
		PhyController * phy;
		LinkPackage * package;

	public:

	private:
		int Send(Packet & pack);

	public:
		LinkController(TYPE type);
		~LinkController();

		int SetBitrate(int bitrate);

		int Send(BYTE * in,int len);
		int Recv(BYTE * out,int maxlen);
};

#endif
