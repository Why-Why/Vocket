/*/////////////////

This is a package class.
Which will input a 01 bits data, get the 01 bits data that will be sended.
And unpacage the data get.
Change BYTE to BIT.
Add the START FLAG (01010100) and END FLAG (01010111).
Add Escape character.
Split the long data.

Note: I use Big-End.

*//////////////////

#ifndef PHYPACKAGE_H
#define PHYPACKAGE_H

#include "Common.h"

class PhyPackage {
	private:
		void Byte2Bit(BIT * out,BYTE b);
		BYTE Bit2Byte(BIT * in);

	public:
		PhyPackage();

		// return the length.
		int Package(BYTE * in,int len,BIT * out);
		int UnPackage(BIT * in,int len,BYTE * out);

		int GetPackageLen(int len);
		int GetUnpackageLen(int len);
};

#endif
