import matplotlib.pyplot as plt
import numpy as np
import struct;

def goertzel(data,freq,rate):
	sp1=0.0;
	sp2=0.0;

	coeff=2*np.cos(2*np.pi*freq/rate);
	for x in data:
		s=x+coeff*sp1-sp2;
		sp2=sp1;
		sp1=s;
	return sp2**2+sp1**2-coeff*sp1*sp2;

def draw(ys):
	for y in ys:
		plt.plot(y);

	plt.show();

def env(data):
	out=[];
	rct=100.0;
	mold=0;
	for x in data:
		if x>mold:
			mold=x;
			out.append(mold);
		else:
			mold*=rct/(rct+1.0);
			out.append(mold);
	return out;

def getPing(x):
	L=len(x);
	winL=4410;
	S=0.0;
	ret=[];

	for i in xrange(0,winL/2):
		ret.append(0);

	for i in xrange(0,winL):
		S+=x[i];

	le=0;
	re=winL;

	for i in xrange(0,L-winL):
		S+=x[re];
		S-=x[le];
		re+=1;
		le+=1;
		ret.append(S/winL);

	return ret;


name=raw_input("PCM File Name: ");
f=open(name,"rb");

num=[];

while 1:
	a=f.read(1);
	if not a: break;
	b=f.read(1);
	if not b: break;

	num.append((struct.unpack('h',b+a))[0]);

winlen=8;

y0=[];
y1=[];
y2=[];

print len(num);

for i in xrange(0,len(num)-winlen):

#	ans=np.fft.fft(num[i:i+winlen]);
#	ty0=np.abs(ans[0])/winlen;
#	ty1=np.abs(ans[256])/winlen;
#	ty2=(np.abs(ans[128])+0)/winlen;

	ty0=0;
	ty1=goertzel(num[i:i+winlen],44100/2,44100);
	ty2=goertzel(num[i:i+winlen],44100/4,44100);

	y0.append(ty0);
	y1.append(ty1);
	y2.append(ty2);

py1=getPing(y1);
py2=getPing(y2);

draw([y1,py1]);
