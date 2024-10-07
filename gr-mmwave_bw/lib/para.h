#ifndef PARA_H
#define PARA_H

#include<vector>
#include<iostream>
#include<complex>
#include <random>
#include <iterator>
#include <fstream>
#include <armadillo>

using namespace std;

#define cpx complex<float>


//for any type in this file refers to the type of vector
//option for cpx,int,float

//key words
//1-dimentional vector
#define vec_1(type) vector<type>
//2-dimentioanl vector
#define vec_2(type) vector<vec_1(type)>
//3-dimentioanl vector
#define vec_3(type) vector<vec_2(type)>

// ways to create vectors,dn for the d-th dimention, num for the number to fill in this vector
//1-dimentional vector
#define new_vec_1(type,d1,num) vector<type>(d1,num)
//2-dimentional vector
#define new_vec_2(type,d1,d2,num) vec_2(type)(d1,new_vec_1(type,d2,num))
//3-dimentional vectpr
#define new_vec_3(type,d1,d2,d3,num) vec_3(type)(d1,new_vec_2(type,d2,d3,num))
/*
class info{

};
*/
class para{
	public:
		int num_ant;
		int fft_size;
		int num_ofdm_per_frame;
		int num_sc_pha_noise;
		int num_payload_ofdm_per_frame;
		int vht_en;
		int num_preamble_ofdm_per_frame;
		int num_samp_per_frame;
		int cp_len;
		int ofdm_size;
		int num_total_ofdm_per_frame;
		float samp_rate;
		
		vec_2(float) freq_pilot;
		//use pilot of 10101010
		
		
		
		vec_1(int) pilot_sc_np;
		vec_1(int) pilot_sc;
		vec_1(int) payload_sc_np;
		vec_1(int) payload_sc;
		vec_1(int) valid_sc_np;
		vec_1(int) valid_sc;
		
		//const number
		const gr_complex pilot=gr_complex(1.7,0);
		//STF for 8bit, LTF for 52 bit including two parts
		
		vec_1(float) time_pilot={1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,
					     1,-1,-1,1,1,-1,-1,1,-1,-1,1,-1,1,-1,-1,1,1,-1,-1,1,-1,-1,1,-1,-1,1,
					     -1,1,1,-1,-1,1,1,-1,1,1,-1,1,-1,1,1,-1,-1,1,1,-1,1,1,-1,1,1,-1
					     };
					     /*
		vec_1(float) time_pilot={1,-1,1,-1,1,-1,1,-1,
					 1,-1,1,-1,1,-1,1,-1,
					 1,-1,1,-1,1,-1,1,-1,
					 1,-1,1,-1,1,-1,1,-1,
					 1,-1,1,-1,1,-1,1,-1,
					 1,-1,1,-1,1,-1,1,-1,
					 1,-1,1,-1,1,-1,1,-1,
					 1,-1,1,-1,1,-1,1,-1
					     };*/
		vec_1(cpx) time_frame_f=new_vec_1(cpx,64,gr_complex(0,0));
		vec_1(cpx) time_frame=new_vec_1(cpx,64,gr_complex(0,0));
		
		vec_1(cpx) time_frame_cp;
		default_random_engine generator;
      		uniform_real_distribution<double> distribution{0.0,1.0};

	public:
		//void initialize(int num_ant, float samp_rate, int fft_size, int num_ofdm_per_frame, int vht_en, int num_sc_pha_noise);
		//float randnum(int min,int max);
		
		void initialize(int num_ant_, float samp_rate_, int fft_size_, int num_ofdm_per_frame_, int vht_en_, int num_sc_pha_noise_){
			num_ant=num_ant_;
			fft_size=fft_size_;
			num_ofdm_per_frame=num_ofdm_per_frame_;
			num_sc_pha_noise=num_sc_pha_noise_;
			samp_rate=samp_rate_;
			vht_en=vht_en_;
			
			for(int i=0;i<time_pilot.size();i++)
				time_frame_f[i]=gr_complex(0.707*time_pilot[i],0.707*time_pilot[i]);
			
			cp_len = 16;
			
			
			
			arma::cx_vec time_frame_ff(fft_size);
			for (int j = 0; j < fft_size; j++)
				time_frame_ff(j) = time_frame_f[j];
			arma::cx_vec time_frame_tt=ifft(time_frame_ff);
			for (int j = 0; j < fft_size; j++)
				time_frame[j] = time_frame_tt[j];
			
			
			time_frame_cp= new_vec_1(cpx,cp_len,gr_complex(0,0));
			for(int i=0;i<cp_len;i++){
				time_frame_cp[i]=time_frame[time_frame.size()-cp_len+i];
			}
			
			
			
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
					{pilot_sc_np.push_back(i);pilot_sc.push_back(i>0?i:i+fft_size);}
				else
					{payload_sc_np.push_back(i);payload_sc.push_back(i>0?i:i+fft_size);}
				valid_sc_np.push_back(i);
				valid_sc.push_back(i>0?i:i+fft_size);
				
			}
			
		}

		float randnum(int min_,int max_){
		      	//std::default_random_engine generator;
		      	//std::uniform_real_distribution<double> distribution(0.0,1.0);
			int min=min_,max=max_;
			return distribution(generator)*(max-min)+min;
		}
		
		

};



#endif
