/*/////////////////

DSTFT mathod for decode.

*//////////////////

#include <algorithm>

#include "Common.h"
#include "MyMath.h"
#include "Physical/DemodulatorDSTFT.h"

using std::max;

DemodulatorDSTFT::DemodulatorDSTFT() {
	Set(DEF_RATE,DEF_RATE0,DEF_RATE1,DEF_WINDOWLEN);
}

DemodulatorDSTFT::DemodulatorDSTFT(int Rate,int Rate0,int Rate1,int WindowLen) {
	Set(Rate,Rate0,Rate1,WindowLen);
}

int DemodulatorDSTFT::Set(int rate,int rate0,int rate1,int windowlen) {
	Rate=rate;
	Rate0=rate0;
	Rate1=rate1;
	WindowLen=windowlen;
	return OK;
}

///////////////////////////////

bool DemodulatorDSTFT::CheckHold(int * sig,int len,int pos,int & L,int & R) {
	for(L=pos;sig[L]==sig[pos];--L);
	++L;
	for(R=pos;sig[R]==sig[pos];++R);
	--R;

	return (R-L+1)>(WindowLen/2)*DEF_HOLD_THRES;

/*
	int cou=0;
	int L=pos-WindowLen/4,R=pos+WindowLen/4;

	for(int i=max(L,0);i<R && i<len;++i)
		if(sig[i]==DEF_STRFLAG[0])
			++cou;

	if(cou>(WindowLen/2)*DEF_HOLD_THRES) return 1;
	return 0;
*/
}

int DemodulatorDSTFT::CheckStr(int * sig,int len,int pos) {
	int sigp=0;
	int tmp,tcou;

	while(pos<len) {
		tcou=WindowLen;
		while(pos<len && sig[pos]==-1 && tcou) ++pos,--tcou;

		if(pos<len && sig[pos]==DEF_STRFLAG[sigp]) ++sigp;
		else break;

		tmp=sig[pos];
		tcou=WindowLen+WindowLen/2;
		while(pos<len && sig[pos]==tmp && tcou) ++pos,--tcou;

		if(sigp>=DEF_STRFLAGLEN) break;
	}

	if(sigp>=DEF_STRFLAGLEN) return pos;
	return -1;

/*
	for(int i=0;i<DEF_STRFLAGLEN;++i,pos+=WindowLen)
		if(sig[pos]!=DEF_STRFLAG[i]) return 0;
	return 1;
*/
}

int DemodulatorDSTFT::FindStr(int * sig,int len) {
	int L,R;

	for(int i=0;i<len;++i) {
		if(sig[i]==DEF_STRFLAG[0]) {
			if(CheckHold(sig,len,i,L,R)) {
				int tmp=CheckStr(sig,len,L);
				if(tmp!=-1) return tmp;
			}
		}
	}

	return -1;
/*
	int step=WindowLen/2;
	int smallstep=WindowLen/8,smalltimes=3;

	for(int i=0;i+WindowLen*DEF_STRFLAGLEN<len;++i)
		if(sig[i]==-1 && sig[i+step]==DEF_STRFLAG[0]) {
			for(int j=0,pos=i+step+step/2;j<smalltimes;++j,pos+=smallstep)
				if(CheckHold(sig,len,pos) && CheckStr(sig,len,pos))
					return pos+WindowLen*DEF_STRFLAGLEN;
			for(int j=1,pos=i+step+step/2-smallstep;j<smalltimes;++j,pos-=smallstep)
				if(CheckHold(sig,len,pos) && CheckStr(sig,len,pos))
					return pos+WindowLen*DEF_STRFLAGLEN;
		}

	return -1;
*/
}

int DemodulatorDSTFT::Decode(int * sig,int len,BIT * out,int str) {
	int ret=0;
	int tmp,times,coulen,tcou;

	double winlen[2]={(double)WindowLen,(double)WindowLen};
	int rcou[2]={1,1};

	while(str<len) {
		tcou=WindowLen;
		while(str<len && sig[str]==-1 && tcou) ++str,--tcou;
		if(sig[str]==-1) break;

		coulen=0;
		tmp=sig[str];
		while(str<len && sig[str]==tmp) ++str,++coulen;

		times=round(coulen/(double)winlen[tmp]);
		for(int i=0;i<times;++i) out[ret++]=tmp;

		// Update winlen.
		winlen[tmp]=(winlen[tmp]*rcou[tmp]+coulen)/(rcou[tmp]+times);
		rcou[tmp]+=times;
	}

	int tp=DEF_ENDFLAGLEN-1;
	for(int i=ret-1;i>=0;--i) {
		if(out[i]==DEF_ENDFLAG[tp]) --tp;
		else tp=DEF_ENDFLAGLEN-1;

		if(tp<0) {
			ret=i;
			break;
		}
	}

	if(tp<0) return ret;
	return ERROR_NOEND;

/*
	int ret=0;
	for(int i=str;i<len;i+=WindowLen) {
		if(sig[i]==-1) break;
		else out[ret++]=sig[i];
	}
	return ret;
*/
}

int DemodulatorDSTFT::Decode(DATA * in,int len,BIT * out) {
	double * spe0=new double [len];
	double * spe1=new double [len];
	int spe0len,spe1len;
	spe0len=GetSpectral(in,len,spe0,Rate0);
	spe1len=GetSpectral(in,len,spe1,Rate1);

	double * ave0=new double [len];
	double * ave1=new double [len];
	int ave0len,ave1len;
	ave0len=GetAverage(spe0,spe0len,ave0,WindowLen/8);
	ave1len=GetAverage(spe1,spe1len,ave1,WindowLen/8);

	int * sig=new int [len+1];
	int siglen;
	sig[0]=-1;
	siglen=GetSig(ave0,ave0len,ave1,ave1len,sig+1)+1;

/*
	FILE *f=fopen("data.txt","w");
	for(int i=0;i<ave0len;++i) fprintf(f,"%lf %lf %d\n",ave0[i],ave1[i],sig[i]*1000000000);
	fclose(f);
*/

	// Get the start of signal.
	int str=FindStr(sig,siglen);
	if(str<0) return ERROR_NOSTART;

	int ret=Decode(sig,siglen,out,str);

	delete [] spe0;
	delete [] spe1;
	delete [] ave0;
	delete [] ave1;
	delete [] sig;

	return ret;
}
