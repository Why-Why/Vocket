#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>

#include "Vocket.h"

using namespace std;

struct Complex {
	double x,y;

	Complex(double a=0.0,double b=0.0):x(a),y(b) {}

	Complex operator + (const Complex & b) const {
		return Complex(x+b.x,y+b.y);
	}
	Complex operator - (const Complex & b) const {
		return Complex(x-b.x,y-b.y);
	}
	Complex operator * (const Complex & b) const {
		return Complex(x*b.x-y*b.y,x*b.y+y*b.x);
	}
};

void change(Complex y[],int len) {
	int k;
	for(int i=1,j=len>>1;i<len-1;++i) {
		if(i<j) swap(y[i],y[j]);
		k=len>>1;
		while(j>=k) {
			j-=k;
			k>>=1;
		}
		if(j<k) j+=k;
	}
}

// len是2的幂，on=1 is DFT，on=-1 is IDFT。
void fft(Complex y[],int len,int on) {
	change(y,len);

	for(int h=2;h<=len;h<<=1) {
		Complex wn(cos(-on*2*PI/h),sin(-on*2*PI/h));
		for(int j=0;j<len;j+=h) {
			Complex w(1,0);
			for(int k=j;k<j+h/2;++k) {
				Complex u=y[k];
				Complex t=w*y[k+h/2];
				y[k]=u+t;
				y[k+h/2]=u-t;
				w=w*wn;
			}
		}
	}

	if(on==-1)
		for(int i=0;i<len;++i)
			y[i].x/=len;
}

Complex ttt[100000];

double cal(int ans[],int len) {
	for(int i=0;i<len;++i) ttt[i]=Complex(ans[i],0);
	for(int i=len;i<2*len;++i) ttt[i]=Complex(0,0);
	fft(ttt,len*2,1);

	cout<<int(ttt[0].x*2/len)<<' '<<int(ttt[1].x*2/len)<<' '<<int(ttt[2].x*2/len)<<' '<<int(ttt[4].x*2/len)<<endl;
}

void solve(int ans[],int len) {
	int WLEN=128;

	for(int i=0;i+WLEN<=len;++i) cal(ans+i,WLEN);
}

DATA ans[1000000];
BIT out[1000000];

// 46064 50793 55648 60502

DATA data[1000000];

char s[1000];

int main() {
/*
	cout<<"Chose Tyte(0 is recv 1 is send): ";
	int x;
	cin>>x;

	if(x==0) {
		Vocket v(VF_FILE);

		while(1) {
			int len=v.Recv(s,1000);
			cout<<len<<endl;
			for(int i=0;i<len;++i) cout<<s[i];
			cout<<endl;
			break;
		}
	}
	else if(x==1) {
		Vocket v(VF_FILE);

		while(1) {
			cin>>s;
			cerr<<"BEGIN\n";
			int len=v.Send(s,strlen(s));
			cerr<<"OK "<<len<<endl;
		}
	}
*/

/*
	AudioALSA aaa;
	AudioPCM bbb("ttt.pcm");

	for(int i=1;i<=5;++i) {
		int len=aaa.Recv(data,44100);
		cerr<<len<<endl;
		bbb.Send(data,len);
	}
*/
/*
	BYTE bbb[]="sfasdgadsgdfadasgasgasgaxx";
	LinkPackage ppp;
	Packet rem[100];

	int len=ppp.Package(bbb,26,rem);
	cout<<len<<endl;
	for(int i=0;i<len;++i) {
		cout<<rem[i].len<<' '<<rem[i].isfinal<<' '<<rem[i].ecc<<endl;
		for(int j=0;j<=rem[i].len;++j) cout<<(int)rem[i].byte[j]<<' ';
	cout<<endl;
	}

	BYTE tttt[1000]={};
	len=ppp.UnPackage(rem,len,tttt);
	cout<<len<<endl;
	cout<<tttt<<endl;
*/
/*
	BIT num[100];

	char name[100];
	cin>>name;
*/
/*
	unsigned char ttt[]={"qwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnm"};
	unsigned char aaa[1000];

	PhyController con;

	int len=con.Recv(aaa,300);
	cerr<<len<<endl;
	for(int i=0;i<len;++i) {
		cout<<aaa[i];
		if(aaa[i]!=ttt[i]) cerr<<"NO\n";
	}
	cout<<endl;
*/
/*
	AudioPCM rrr(name);
	int len=rrr.Recv(ans,1000000);

	cout<<len<<endl;
	for(int i=0;i<20;++i) cout<<ans[i]<<' ';
	cout<<endl;
*/
/*
	DemodulatorDSTFT ddd(44100,2,4,882);
	int dlen=ddd.Decode(ans,len,out);

	cout<<dlen<<endl;
	for(int i=0;i<dlen;++i) {
		cout<<out[i]<<',';
		if((i+1)%8==0) cout<<endl;
	}
	cout<<endl;
*/
/*
	for(int i=0;i<100;++i) num[i]=rand()%2;

	ModulatorFSK fsk;
	int len=fsk.Encode(num,100,ans);

	for(int i=0;i<100;++i) cerr<<num[i]<<' ';
	cerr<<endl;
*/
/*
	int ttans[100000];
	for(int i=0;i<len;++i) ttans[i]=ans[i];
	solve(ttans,len);

	return 0;
*/
/*
	AudioPCM ttt("zz.pcm");
	ttt.Send(ans,len);

	AudioPCM rrr("zz.pcm");
	rrr.Recv(tans,len);

	for(int i=0;i<len;++i)
		if(ans[i]!=tans[i]) cerr<<"NO\n";
	cerr<<"OK\n";
*/

	return 0;
}
