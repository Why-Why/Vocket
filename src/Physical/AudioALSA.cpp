/*////////////////////

*/////////////////////

#include "Common.h"
#include "Physical/AudioALSA.h"

#include <unistd.h>

AudioALSA::AudioALSA() {
	inopen=outopen=0;
}

AudioALSA::~AudioALSA() {
	if(inopen) {
		inopen=0;
		snd_pcm_drain(handle_in);
		snd_pcm_close(handle_in);
	}

	if(outopen) {
		outopen=0;
		snd_pcm_drain(handle_out);
		snd_pcm_close(handle_out);
	}
}

void AudioALSA::InitOut() {
	if(outopen) return;
	outopen=1;

	snd_pcm_hw_params_t * params;
	snd_pcm_open(&handle_out,"default",SND_PCM_STREAM_PLAYBACK,0);
	snd_pcm_hw_params_alloca(&params);
	snd_pcm_hw_params_any(handle_out,params);
	snd_pcm_hw_params_set_access(handle_out,params,SND_PCM_ACCESS_RW_INTERLEAVED);
	snd_pcm_hw_params_set_format(handle_out,params,SND_PCM_FORMAT_S16_BE);
	snd_pcm_hw_params_set_channels(handle_out,params,1);

	unsigned int rate=DEF_RATE;		// Maybe error, shouble be the same with ModulatorFSK's rate.
	snd_pcm_hw_params_set_rate_near(handle_out,params,&rate,0);

	snd_pcm_uframes_t frames=32;
	snd_pcm_hw_params_set_period_size_near(handle_out,params,&frames,0);

	snd_pcm_hw_params(handle_out,params);

	snd_pcm_hw_params_get_period_size(params,&frames,0);
	frames_out=frames;
}

void AudioALSA::InitIn() {
	if(inopen) return;
	inopen=1;

	snd_pcm_hw_params_t * params;
	snd_pcm_open(&handle_in,"default",SND_PCM_STREAM_CAPTURE,0);
	snd_pcm_hw_params_alloca(&params);
	snd_pcm_hw_params_any(handle_in,params);
	snd_pcm_hw_params_set_access(handle_in,params,SND_PCM_ACCESS_RW_INTERLEAVED);
	snd_pcm_hw_params_set_format(handle_in,params,SND_PCM_FORMAT_S16_BE);
	snd_pcm_hw_params_set_channels(handle_in,params,1);

	unsigned int rate=DEF_RATE;		// Maybe error, shouble be the same with ModulatorFSK's rate.
	snd_pcm_hw_params_set_rate_near(handle_in,params,&rate,0);

	snd_pcm_hw_params(handle_in,params);

	snd_pcm_hw_params_get_period_size(params,&frames_in,0);
}

int AudioALSA::Send(DATA * data,int len) {
	if(outopen==0) InitOut();

	snd_pcm_prepare(handle_out);
	BYTE * buf=new BYTE [frames_out*2];

	for(int i=0;i<len;) {
		int size=0;
		for(int j=0;j<frames_out && i<len;++i,++j) {
			buf[size++]=BYTE((data[i]>>8) & 0xff);
			buf[size++]=BYTE(data[i] & 0xff);
		}

		snd_pcm_writei(handle_out,buf,size);
	}

	sleep(len/(double)DEF_RATE);		// !!!!

	delete [] buf;
	return len;
}

int AudioALSA::Recv(DATA * data,int maxlen) {
	if(inopen==0) InitIn();

	unsigned char * buf=new unsigned char [maxlen*2];

	snd_pcm_prepare(handle_in);
	int len=snd_pcm_readi(handle_in,buf,maxlen);
	short a,b;

	for(int i=0;i<len;++i) {
		a=buf[i<<1];
		b=buf[(i<<1)|1];
		data[i]=(a<<8)|b;
	}

	delete [] buf;
	return len;
}
