/* -*- c++ -*- */
/*
 * Copyright 2023 bowei.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "tx_time_frame_impl.h"

namespace gr {
  namespace mmwave_bw {

    using input_type = gr_complex;
    using output_type = gr_complex;
    tx_time_frame::sptr
    tx_time_frame::make(int num_ant_,float samp_rate_,int fft_size_,int num_ofdm_per_frame_,int vht_en_,int num_sc_pha_noise_)
    {
      return gnuradio::make_block_sptr<tx_time_frame_impl>(
        num_ant_, samp_rate_, fft_size_, num_ofdm_per_frame_, vht_en_, num_sc_pha_noise_);
    }


    /*
     * The private constructor
     */
    tx_time_frame_impl::tx_time_frame_impl(int num_ant_,float samp_rate_,int fft_size_,int num_ofdm_per_frame_,int vht_en_,int num_sc_pha_noise_)
      : gr::block("tx_time_frame",
              gr::io_signature::make(1 /* min inputs */, 8 /* max inputs */, sizeof(input_type)),
              gr::io_signature::make(1 /* min outputs */, 8 /*max outputs */, sizeof(output_type)))
    {
    	p.initialize(num_ant_,samp_rate_,fft_size_,num_ofdm_per_frame_,vht_en_,num_sc_pha_noise_);
    	set_output_multiple(p.ofdm_size*p.num_total_ofdm_per_frame);
    }

    /*
     * Our virtual destructor.
     */
    tx_time_frame_impl::~tx_time_frame_impl()
    {
    }

    void
    tx_time_frame_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {

      ninput_items_required[0] = p.fft_size*p.num_payload_ofdm_per_frame;
    }

    int
    tx_time_frame_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      input_type* in[8];
      output_type* out[8];
	for (int n = 0; n < p.num_ant; n++) {
		in[n] = (gr_complex *) input_items[n];
		out[n] = (gr_complex *) output_items[n];
	}
	int index=0;//keep track of number of inserted number in output
	int index_in=0;//keep track of number of inserted number in input
	for (int n=0;n<p.num_ant;n++){
		index=0;
		index_in=0;
		//insert preamble
		for(int k =0;k<p.num_preamble_ofdm_per_frame;k++){
			//insert cp of preamble
			for(int m=0;m<p.cp_len;m++)
				out[n][index++]=p.time_frame_cp[m];
			//insert preamble
			for(int m=0;m<p.time_frame.size();m++)
				out[n][index++]=p.time_frame[m];	
		}
		
		//insert data
		for(int k =0;k<p.num_payload_ofdm_per_frame;k++){
			//insert cp of data
			for(int m=0;m<p.cp_len;m++)
				out[n][index++]=in[n][k*p.fft_size+p.fft_size-p.cp_len+m];
			for(int m=0;m<p.fft_size;m++)
				out[n][index++]=in[n][index_in++];
		}
	}
      
      vec_1(cpx) zeros=new_vec_1(cpx,p.cp_len+p.fft_size,gr_complex(0,0));
      for(int i=0;i<p.ofdm_size;i++)
      	zeros[i]=gr_complex(0,0);
      if(p.num_ant>1){
      	for(int n=0;n<p.num_ant;n++){
      		for(int i=0;i<p.ofdm_size;i++)
      			out[n][(4+n)*p.ofdm_size+i]=gr_complex(0,0);
      		//memcpy(&out[n][(3+p.num_ant)*p.ofdm_size],&zeros,sizeof(gr_complex)*p.ofdm_size);
      	}
      }	
		
      consume_each (p.num_total_ofdm_per_frame*p.fft_size);

      // Tell runtime system how many output items we produced.
      return p.num_total_ofdm_per_frame*p.ofdm_size;
    }

  } /* namespace mmwave_bw */
} /* namespace gr */
