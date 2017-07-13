#include "Vocket.h"

#include <iostream>
#include <cstdio>
#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

Vocket v;

int InitSock() {
	int sock=socket(AF_INET,SOCK_STREAM,0);

	int val=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val));

	int port=11111;
	sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(port);
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	memset(&(addr.sin_zero),0,8);
	int rc=bind(sock,(sockaddr *)&addr,sizeof(sockaddr));

	if(rc==-1) {
		cerr<<"NOBIND\n";
		return 0;
	}

	cerr<<"START LISTEN\n";

	rc=listen(sock,5);
	if(rc==-1) {
		cerr<<"NOLIS\n";
		return 0;
	}

	sockaddr_in client;
	int tmp,size=sizeof(sockaddr_in);
	tmp=accept(sock,(sockaddr *)&client,(socklen_t*)&size);
	sock=tmp;

	cerr<<"OK\n";

	return sock;
}

BYTE tbuf[10000000];
int SOCK;

int Send(DATA * data,int len) {
	int cou=0;
	tbuf[cou++]='4';

	for(int i=0;i<len;++i) {
		tbuf[cou++]=BYTE((data[i]>>8) & 0xff);
		tbuf[cou++]=BYTE(data[i] & 0xff);
	}

	tbuf[cou++]='\n';
	send(SOCK,tbuf,cou,0);
}

void StartSend(int sock,char buf[],int len) {
	cerr<<"Start Send\n";

	SOCK=sock;
	v.SendToFunc((void *)buf,len,Send);
	send(SOCK,"5\n",2,0);

	cerr<<"Finish Send\n";
}

DATA data[1000000];
char tmp[1000];
FILE * f;

int Recv(DATA * data,int len) {
	int step=1024;
	int cou=0;

	cerr<<len<<endl;

	while(cou<len) {
		int flag=min((len-cou)*2,step);

		sprintf(tmp,"6%d\n",flag);
		send(SOCK,tmp,strlen(tmp),0);
		int tmplen=recv(SOCK,tbuf,flag,0);

		for(int i=0;i+1<tmplen;i+=2) {
			short a=tbuf[i],b=tbuf[i+1];
			data[cou++]=(a<<8)|b;
		}

		if(tmplen!=flag) {
			break;
		}
	}

	cerr<<"# "<<cou<<endl;

	return cou;
}

char buf[1000000];

void StartRecv(int sock) {
	cerr<<"Start Recv\n";
	SOCK=sock;
	int len=v.RecvFromFunc(buf+1,100000,Recv);

	cerr<<"Recv OK\n";
	cerr<<len<<endl;
	for(int i=0;i<len;++i) cerr<<buf[i];
	cerr<<endl;
	cerr<<"####\n";

	buf[0]='7';
	if(len>0) send(SOCK,buf,len+1,0);
	else {
		buf[1]='n';
		buf[2]='o';
		buf[3]='\n';
		send(SOCK,buf,4,0);
	}

	cerr<<"Finish Recv\n";
}

int main() {
	int sock=InitSock();

	while(1) {
		cerr<<"Start Wait\n";
		int len=recv(sock,buf,1000,0);

		for(int i=0;i<len;++i) cerr<<(int)buf[i]<<' ';
		cerr<<endl;

		if(buf[0]=='1') StartSend(sock,buf+1,len-1);
		else if(buf[0]=='2') StartRecv(sock);
		else break;
	}

	close(sock);

	return 0;
}
