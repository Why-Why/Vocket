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

	Init();

	return OK;
}

int DemodulatorDSTFT::SetBitrate(int bitrate) {
	WindowLen=(Rate+bitrate-1)/bitrate;

	Init();

	return OK;
}

void DemodulatorDSTFT::Init() {
	ActWinLen[0]=-1;
	ActWinLen[1]=-1;
	SigCou[0]=0;
	SigCou[1]=0;
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
/*
		tcou=WindowLen;
		while(pos<len && sig[pos]==-1 && tcou) ++pos,--tcou;
*/
/*
		if(pos<len && sig[pos]==DEF_STRFLAG[sigp]) ++sigp;
		else break;
*/

		tmp=sig[pos];
		tcou=0;
		while(pos<len && sig[pos]==tmp) ++pos,++tcou;

		if(tcou>=WindowLen) {
			int times;

			if(ActWinLen[tmp]<0) {
				times=1;
				ActWinLen[tmp]=tcou;
				SigCou[tmp]=1;
			}
			else {
				times=round(tcou/ActWinLen[tmp]);
				if(tcou/(double)times>=WindowLen) ActWinLen[tmp]=(ActWinLen[tmp]*SigCou[tmp]+tcou)/(SigCou[tmp]+times);
				SigCou[tmp]+=times;
			}

			for(int i=0;i<times && sigp<DEF_STRFLAGLEN;++i) {
				if(DEF_STRFLAG[sigp]==tmp) ++sigp;
				else goto finish;
			}
		}
		else break;

		if(sigp>=DEF_STRFLAGLEN) break;
	}

finish:

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

	Init();		// !!!

	for(int i=0;i<len;++i) {
		if(sig[i]==DEF_STRFLAG[0]) {
			if(CheckHold(sig,len,i,L,R)) {
				int tmp=CheckStr(sig,len,L);
				if(tmp!=-1) return L;
				else i+=WindowLen;			// Maybe error.
			}
			else i+=WindowLen;				// Maybe error.
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

int DemodulatorDSTFT::Data2Sig(DATA * in,int len,int * sig) {
	double * spe0=new double [len];
	double * spe1=new double [len];
	int spe0len,spe1len;
	spe0len=GetSpectral(in,len,spe0,Rate0);
	spe1len=GetSpectral(in,len,spe1,Rate1);

	double * ave0=new double [len];
	double * ave1=new double [len];
	int ave0len,ave1len;
	ave0len=GetAverage(spe0,spe0len,ave0,WindowLen/6);
	ave1len=GetAverage(spe1,spe1len,ave1,WindowLen/6);

	int * tmpsig=new int [len];
	int tmpsiglen=GetSig(ave0,ave0len,ave1,ave1len,tmpsig);
/*
static int cou=0;
++cou;
char name[100];
sprintf(name,"Y%d",cou);
FILE * f=fopen(name,"w");
*/
	// Get an average of signal, avoid some noise.
	int siglen=GetAveSig(tmpsig,tmpsiglen,sig,WindowLen);
/*
for(int i=0;i<siglen;++i) fprintf(f,"%lf %lf %d\n",ave0[i],ave1[i],tmpsig[i]*1000000000);
fclose(f);
*/
	delete [] spe0;
	delete [] spe1;
	delete [] ave0;
	delete [] ave1;

	return siglen;
}

// Note: Bug: if the end is 000000 or 111111, this will be passed.
int DemodulatorDSTFT::Decode(int * sig,int len,BIT * out,int str,int & nextstr) {
	int ret=0;
	int tmp,times,coulen,tcou;

	int maxlen=len-4*WindowLen;

	while(str<maxlen) {
/*
		tcou=WindowLen;
		while(str<len && sig[str]==-1 && tcou) ++str,--tcou;
		if(sig[str]==-1) {
			nextstr=-1;
			break;
		}
*/
		nextstr=str;
		coulen=0;
		tmp=sig[str];
		while(str<len && sig[str]==tmp) ++str,++coulen;

		// Do not have a long 0 or 1 !!!!!!!
		// Or there will be something wrong.
		if(str>=maxlen) break;

		// I dont konw why the final voice wave's WindowLen will be longer, even 2*original WindowLen.
		if(ActWinLen[tmp]<0)
			break;		// Error !!!
		else
			times=round(coulen/(double)ActWinLen[tmp]);

		for(int i=0;i<times;++i) out[ret++]=tmp;

		// Update winlen.
		if(coulen/(double)times>=WindowLen) ActWinLen[tmp]=(ActWinLen[tmp]*SigCou[tmp]+coulen)/(SigCou[tmp]+times);
		SigCou[tmp]+=times;
	}

	return ret;

/*
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
*/
/*
	int ret=0;
	for(int i=str;i<len;i+=WindowLen) {
		if(sig[i]==-1) break;
		else out[ret++]=sig[i];
	}
	return ret;
*/
}

int DemodulatorDSTFT::GetLastdataLen() {
	return WindowLen*(DEF_STRFLAGLEN+1);
}

int DemodulatorDSTFT::GetDataLen(int siglen) {
//	return siglen+DEF_GOELEN;		// ???
	return siglen;
}

int DemodulatorDSTFT::GetSigLen(int datalen) {
	return datalen;
}

int DemodulatorDSTFT::GetBitLen(int siglen) {
	return (siglen+WindowLen-1)/WindowLen;
}
