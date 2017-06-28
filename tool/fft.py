import matplotlib.pyplot as plt
import numpy as np
import struct;

def draw(ys):
	for y in ys:
		plt.plot(y);

	plt.show();

name=raw_input("PCM File Name: ");
f=open(name,"rb");

num=[];

while 1:
	a=f.read(1);
	if not a: break;
	b=f.read(1);
	if not b: break;

	num.append((struct.unpack('h',b+a))[0]);

winlen=128;

y1=[];
y2=[];

for i in range(0,len(num)-winlen):
	ans=np.fft.fft(num[i:i+winlen]);
	ty1=np.abs(ans[1])/128;
	ty2=np.abs(ans[2])/128;

	y1.append(ty1);
	y2.append(ty2);

draw([y1,y2]);
