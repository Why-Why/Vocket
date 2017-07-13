/*////////////////////

*/////////////////////

#include "Common.h"
#include "Physical/PhyPackage.h"

PhyPackage::PhyPackage() {
}

// Big-End.
void PhyPackage::Byte2Bit(BIT * out,BYTE b) {
	for(int i=0;i<8;++i) {
		if(b&(1<<i)) out[7-i]=1;
		else out[7-i]=0;
	}
}

BYTE PhyPackage::Bit2Byte(BIT * in) {
	BYTE ret=0;
	for(int i=0;i<8;++i)
		if(in[7-i]) ret|=(1<<i);
	return ret;
}

int PhyPackage::Package(BYTE * in,int len,BIT * out) {
	int ret=0;

	Byte2Bit(out+ret,DEF_STRFLAG_BYTE);
	ret+=8;

	// This is not good, I should have a fixed len of data to be sended, instead of escape character。
	for(int i=0;i<len;++i) {
		if(in[i]==DEF_STRFLAG_BYTE || in[i]==DEF_ENDFLAG_BYTE || in[i]==DEF_FLAG_BYTE) {
			Byte2Bit(out+ret,DEF_FLAG_BYTE);
			ret+=8;
		}
		Byte2Bit(out+ret,in[i]);
		ret+=8;
	}

	Byte2Bit(out+ret,DEF_ENDFLAG_BYTE);
	ret+=8;
	Byte2Bit(out+ret,0);
	ret+=8;

	return ret;
}

int PhyPackage::UnPackage(BIT * in,int len,BYTE * out) {
	int ret=0;
	BYTE tmp;
	bool haveend=0,haveflag=0;

//	if(Bit2Byte(in)!=DEF_STRFLAG_BYTE) return ERROR_NOSTART;

	for(int i=0;i+8<=len;i+=8) {
		tmp=Bit2Byte(in+i);
		if(haveflag) {
			out[ret++]=tmp;
			haveflag=0;
		}
		else {
			if(tmp==DEF_FLAG_BYTE)
				haveflag=1;
			else if(tmp==DEF_ENDFLAG_BYTE) {
				haveend=1;
				break;
			}
			else out[ret++]=tmp;
		}
	}

	return ret;
/*
	if(haveend) return ret;
	else return ERROR_NOEND;
*/
}

int PhyPackage::GetPackageLen(int len) {
	return (len*2+2)*8;
}

int PhyPackage::GetUnpackageLen(int len) {
	return (len+7)/8;
}
