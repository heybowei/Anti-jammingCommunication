#include "para.h"
using namespace std;


void para::initialize(int num_ant_, float samp_rate_, int fft_size_, int num_ofdm_per_frame_, int vht_en_, int num_sc_pha_noise_){
	num_ant=num_ant_;
	fft_size=fft_size_;
	num_ofdm_per_frame_=num_ofdm_per_frame;
	num_sc_pha_noise_=num_sc_pha_noise;
	samp_rate=samp_rate_;
	vht_en=vht_en_;
	
	cp_len = 16;
	ofdm_size = cp_len + fft_size;
	vht_en = vht_en_;

	if (vht_en == 0)
		num_preamble_ofdm_per_frame = 4; 
	else 
		num_preamble_ofdm_per_frame = 4 + num_ant;
	
	num_total_ofdm_per_frame = num_ofdm_per_frame_;
	num_payload_ofdm_per_frame = num_total_ofdm_per_frame - num_preamble_ofdm_per_frame;
	num_samp_per_frame = ofdm_size*num_total_ofdm_per_frame;
	
	if (num_payload_ofdm_per_frame < 1) num_payload_ofdm_per_frame = 1;


	if (num_sc_pha_noise > 6) num_sc_pha_noise = 6;

	for(int i=-fft_size/2+int(6*fft_size/64);i<fft_size/2-int(6*fft_size/64);i++){
		if(i==0||abs(i)<=num_sc_pha_noise) continue;
		if((i+7)%14==0) 
			pilot_sc.push_back(i>=0?i:i+fft_size);
		else
			payload_sc.push_back(i>=0?i:i+fft_size);
		valid_sc.push_back(i>=0?i:i+fft_size);
	}
	
}

float para::randnum(int min_,int max_){
      	//std::default_random_engine generator;
      	//std::uniform_real_distribution<double> distribution(0.0,1.0);
	int min=min_,max=max_;
	return distribution(generator)*(max-min)+min;
}
