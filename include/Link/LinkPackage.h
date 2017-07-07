/*//////////////////////

The Package in Link Layer.
1. Split the long data.
2. Add CheckSum (ECC).

The class will split thee package to 8 Byte each package.
And you can get more details from the "doc/LinkPackageStruct.png".

*///////////////////////

#ifndef LINKPACKAGE_H
#define LINKPACKAGE_H

#include "Common.h"

struct Packet {
	unsigned short len : 3;		// !!! len is the actual length-1 !!!
	unsigned short isfinal : 1;
	unsigned short ecc : 12;

	BYTE byte[8];

	// About ECC, Wiki and "doc/ECC.png" for more details.
	int GetECC();
	int CheckECC();

	private:
		int GetXOR(int x,int str,int step,int last);
};

class LinkPackage {
	private:

	public:

	private:
		int PackageOne(BYTE * in,int len,Packet & out);

	public:
		LinkPackage();

		// return the number of package.
		int Package(BYTE * in,int len,Packet * out);
		// return the length of byte, Just Unpackage one packet each time.
		int UnPackage(Packet * in,BYTE * out,bool & isfinal);

		// return the number of package needed for the len bytes.
		int GetPackageLen(int bytelen);
		// return the size of Packet.
		int GetByteLen();
};

#endif
