/*/////////////////

*//////////////////

#include <algorithm>

#include "Common.h"
#include "Link/LinkPackage.h"

using std::min;

LinkPackage::LinkPackage() {
}

int LinkPackage::Package(BYTE * in,int len,Packet * out) {
	int ret=0;

	for(int i=0;i<len;i+=DEF_LINKPACKAGE) {
		int L=i,R=min(i+DEF_LINKPACKAGE,len);

		PackageOne(in+L,R-L,out[ret]);
		out[ret].isfinal=(R==len ? 1 : 0);

		++ret;
	}

	return ret;
}

int LinkPackage::PackageOne(BYTE * in,int len,Packet & out) {
	for(int i=0;i<8;++i) out.byte[i]=0;

	out.len=(len-1)&7;
	for(int i=0;i<len;++i) out.byte[i]=in[i];
	out.ecc=out.GetECC();
}

int LinkPackage::GetPackageLen(int bytelen) {
	return (bytelen+7)/8;
}

int LinkPackage::UnPackage(Packet * in,BYTE * out,bool & isfinal) {
	isfinal=in->isfinal;
	for(int i=0;i<=in->len;++i) out[i]=in->byte[i];
	return in->len+1;

/*
	for(int i=0;i<len;++i) {
		if(in[i].CheckECC()==ERROR_ERRORDATA) return ERROR_ERRORDATA;

		for(int j=0;j<in[i].len+1;++j)
			out[ret++]=in[i].byte[j];
		if(in[i].isfinal) {
			haveend=1;
			break;
		}
	}

	if(haveend) return ret;
	return ret;		// Error no end.
*/
}

////////////////

int Packet::GetXOR(int x,int str,int step,int last) {
	int ret=0;
	for(int i=str;i<8;i+=step) {
		for(int j=0;j<last;++j) if(x & (1<<(i+j))) ret^=1;
	}
	return ret;
}

int Packet::GetECC() {
	int alen=len+1;
	BYTE col=0,row=0;
	int tmp;

	for(int i=0;i<alen;++i) {
		col^=byte[i];
		for(int tmp=byte[i];tmp;tmp^=(tmp & (-tmp))) row^=(1<<i);
	}

	unsigned short ecc=0;
	ecc|=GetXOR(col,0,2,1)|(GetXOR(col,1,2,1)<<1)|(GetXOR(col,0,4,2)<<2)|(GetXOR(col,2,4,2)<<3)|(GetXOR(col,0,8,4)<<4)|(GetXOR(col,4,8,4)<<5);
	ecc|=(GetXOR(row,0,2,1)<<6)|(GetXOR(row,1,2,1)<<7)|(GetXOR(row,0,4,2)<<8)|(GetXOR(row,2,4,2)<<9)|(GetXOR(row,0,8,4)<<10)|(GetXOR(row,4,8,4)<<11);

	return ecc;
}

int Packet::CheckECC() {
	int actecc=GetECC();
	if(actecc==ecc) return OK;

	int cou=0;
	int rem[12];
	for(int i=0;i<12;++i)
		if((actecc&(1<<i))!=(ecc&(1<<i))) rem[cou++]=i;
	if(cou!=6) return ERROR_ERRORDATA;

	int r=0,c=0;
	for(int i=0;i<3;++i) {
		if(rem[i]<2*i || rem[i]>=2*i+2) return ERROR_ERRORDATA;
		if(rem[i]&1) c|=(1<<i);
	}

	for(int i=3;i<6;++i) {
		if(rem[i]<2*i || rem[i]>=2*i+2) return ERROR_ERRORDATA;
		if(rem[i]&1) r|=(1<<(i-3));
	}

	byte[r]^=(1<<c);
	return OK;
}

int LinkPackage::GetByteLen() {
	return sizeof(Packet);
}
