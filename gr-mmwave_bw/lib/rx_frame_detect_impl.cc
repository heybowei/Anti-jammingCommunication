/* -*- c++ -*- */
/*
 * Copyright 2023 bowei.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "rx_frame_detect_impl.h"

namespace gr {
  namespace mmwave_bw {

    using input_type = gr_complex;
    using output_type = gr_complex;
    rx_frame_detect::sptr
    rx_frame_detect::make(int num_ant, int num_stm, float samp_rate, int fft_size, int num_ofdm_per_frame, int vht_en, int project_en, int search_step_size, float noise_ampl_thres, float ltf_corr_thres, int freq_offset_en, int num_sc_pha_noise)
    {
      return gnuradio::make_block_sptr<rx_frame_detect_impl>(
        num_ant, num_stm, samp_rate, fft_size, num_ofdm_per_frame, vht_en, project_en, search_step_size, noise_ampl_thres, ltf_corr_thres, freq_offset_en, num_sc_pha_noise);
    }


    /*
     * The private constructor
     */
    rx_frame_detect_impl::rx_frame_detect_impl(int num_ant_, int num_stm_, float samp_rate_, int fft_size_, int num_ofdm_per_frame_, int vht_en_, int project_en_, int search_step_size_, float noise_ampl_thres_, float ltf_corr_thres_, int freq_offset_en_, int num_sc_pha_noise_)
      : gr::block("rx_frame_detect",
              gr::io_signature::make(1 /* min inputs */, 8 /* max inputs */, sizeof(input_type)),
              gr::io_signature::make(1 /* min outputs */, 8 /*max outputs */, sizeof(output_type)))
    {
    	p.initialize(num_ant_,samp_rate_,fft_size_,num_ofdm_per_frame_,vht_en_,num_sc_pha_noise_);
    	noise_ampl_thres=noise_ampl_thres_;
    	project_en=project_en_;
    	ltf_corr_thres=ltf_corr_thres_;
    	freq_offset_en=freq_offset_en_;
    	set_output_multiple(p.num_samp_per_frame+1);
    }

    /*
     * Our virtual destructor.
     */
    rx_frame_detect_impl::~rx_frame_detect_impl()
    {
    }

    void
    rx_frame_detect_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      for(int n=0;n<p.num_ant;n++)
      	ninput_items_required[n] = p.num_samp_per_frame*2;		//to ensure there is at least on complete frame and some buffer data
    }

    int
    rx_frame_detect_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      for(int n=0;n<p.num_ant;n++){
        in[n] = (input_type *) input_items[n];
      	out[n] = (output_type *) output_items[n];
      }
      
      if(abs(in[0][0]-in[0][1])<noise_ampl_thres)
      	{consume_each(1);return 0;}
/*
      if (project_en == 0){
      	find_preamble();
      }
      */
      for(int n=0;n<p.num_ant;n++){
      	if (project_en == 0){
      		start_index=find_preamble(n);
      		memcpy(&out[n][0],&in[n][start_index],sizeof(gr_complex)*p.num_samp_per_frame);
      		out[n][p.num_samp_per_frame]=gr_complex(freq_offset,0);
      	
      	}
      }
      //exit(0);
      consume_each (start_index+p.num_samp_per_frame);
      
      
      
      
      // Tell runtime system how many output items we produced.
      return p.num_samp_per_frame+1;//p.num_samp_per_frame;
    }
    
    float rx_frame_detect_impl::find_preamble(int n){
   	//initialize to store
   	int start=n;
    	max_val=0;
    	start_index=0;
    	//cout<<"\n-----------------------------------------";
    	//do correlation between in[][] and time_pilot[]
    	c_num.clear();
    	c_num = new_vec_1(float,2*p.num_samp_per_frame+p.time_frame.size()+1,0);
    	gr_complex cor,signal;
    	int cor_len=2*p.num_samp_per_frame;
    	for(int i=0;i<cor_len;i++){
    		cor=gr_complex(0,0);
    		signal=gr_complex(0,0);
    		for(int j=0;j<p.time_frame.size();j++){
    			cor+=in[start][i+j]*conj(p.time_frame[j]);
    			signal+=in[start][i+j]*conj(in[start][i+j]);
    		}
    		c_num[i]=abs(cor)/sqrt(abs(signal));
    		
    		if(i>p.ofdm_size && c_num[i]>ltf_corr_thres && c_num[i-p.ofdm_size]>ltf_corr_thres)
    			cor_len=i;//,cout<<ltf_corr_thres;//,cout<<i<<"->"<<c_num[i]<<"-->"<<c_num[i-p.ofdm_size];
		//if(c_num[i]>ltf_corr_thres)
		//	cor_len=i;
    	}
    	
    	double max_cor=-1;
    	int max_index=-1;
    	for(int i=0;i<cor_len;i++){
    		if(c_num[i]>max_cor){
    			max_cor=c_num[i];
    			max_index=i;
    		}
    	}
    	//cout<<max_index<<"--->";
    	if(max_cor>ltf_corr_thres){
    		max_val=max_cor;
    		start_index=max_index-p.cp_len;//+(p.ofdm_size*2+p.cp_len);
    		
    		if(freq_offset_en != 0) freq_offset_estimation(start_index,start);
    	}
    	return start_index;
    }
    
    
    
    void rx_frame_detect_impl::freq_offset_estimation(int start_index, int start){
    	//initialize to store
    	int n=start;
    	freq_offset=0;
    	gr_complex corr_freq=gr_complex(0,0);
    	for(int i=n;i<n;i++)
    		for(int j=0;j<p.num_total_ofdm_per_frame-1;j++)
    			for(int k=0;k<p.cp_len;k++){
    				corr_freq+=in[i][start_index+j*p.ofdm_size+k]*conj(in[i][start_index+j*p.ofdm_size+k+p.fft_size]);
    				//cout<<in[i][start_index+j*p.ofdm_size+k]-in[i][start_index+j*p.ofdm_size+k+p.fft_size]<<"\n";
    				//if(abs(in[i][start_index+j*p.ofdm_size+k]-in[i][start_index+j*p.ofdm_size+k+p.fft_size])>0)
    				//	cout<<start_index+j*p.ofdm_size+k<<"  ->"<<start_index+j*p.ofdm_size+k+p.fft_size<<"\n";
    			}
    				
    	freq_offset=arg(corr_freq)/p.fft_size;
    	//cout<<corr_freq<<"->"<<freq_offset<<"\n";
    }
    
    
  } /* namespace mmwave_bw */
} /* namespace gr */
