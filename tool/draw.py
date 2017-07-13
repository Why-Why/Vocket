import matplotlib.pyplot as plt

name=raw_input("FILE NAME: ");
num=int(raw_input("NUM: "));
f=open(name,"r");

y=[];
for i in range(0,num):
	y.append([]);

while 1:
	s=f.readline();
	if not s: break;
	s=s.split();
	for i in xrange(0,num):
		y[i].append(float(s[i]));

for ty in y:
	plt.plot(ty);
plt.show();
