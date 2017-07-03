/*/////////////////////////

*//////////////////////////

#include <cmath>
#include <algorithm>

#include "Common.h"
#include "MyMath.h"

using std::cos;
using std::max;
using std::min;

double Goertzel(DATA * data,int len,int Rate0) {
	double s,sp1=0,sp2=0;
	double coeff=2.0*cos(2.0*PI/Rate0);

	for(int i=0;i<len;++i) {
		s=data[i]+coeff*sp1-sp2;
		sp2=sp1;
		sp1=s;
	}

	return sp2*sp2+sp1*sp1-coeff*sp1*sp2;
}

int GetSpectral(DATA * data,int len,double * out,int Rate0,int Goelen) {
	int ret=0;
	for(int i=0;i+Goelen<=len;++i)
		out[ret++]=Goertzel(data+i,Goelen,Rate0);
	return ret;
}

int GetAverage(double * data,int len,double * out,int Avelen) {
	int ret=0;
	double sum=0;

	for(int i=0;i<Avelen;++i) {
		sum+=data[i];
		out[ret++]=sum/Avelen;
	}
	for(int i=Avelen,L=0,R=Avelen;i<len;++i) {
		out[ret++]=sum/Avelen;
		++R;
		sum+=data[R];
		sum-=data[L];
		++L;
	}

	return ret;
}

int GetSig(double * data0,int len0,double * data1,int len1,int * out) {
	int ret=min(len0,len1);
	double x,y,k;

	for(int i=0;i<ret;++i) {
		x=fabs(data0[i]);
		y=fabs(data1[i]);
		k=max(x,y)/min(x,y);

		if(max(x,y)>DEF_SIG_THRES && k>DEF_SIGMUL_THRES) {
			if(x>y) out[i]=0;
			else out[i]=1;
		}
		else out[i]=-1;
	}

	return ret;
}
