/* -*- c++ -*- */
/*
 * Copyright 2023 bowei.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "tx_freq_frame_impl.h"

namespace gr {
  namespace mmwave_bw {

    using input_type = gr_complex;
    using output_type = gr_complex;
    tx_freq_frame::sptr
    tx_freq_frame::make(int num_ant_,float samp_rate,int fft_size_,int num_ofdm_per_frame_,int vht_en,int num_sc_pha_noise)
    {
      return gnuradio::make_block_sptr<tx_freq_frame_impl>(
        num_ant_, samp_rate, fft_size_, num_ofdm_per_frame_, vht_en, num_sc_pha_noise);
    }


    /*
     * The private constructor
     */
    tx_freq_frame_impl::tx_freq_frame_impl(int num_ant_,float samp_rate,int fft_size_,int num_ofdm_per_frame_,int vht_en,int num_sc_pha_noise)
      : gr::block("tx_freq_frame",
              gr::io_signature::make(1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
              gr::io_signature::make(1 /* min outputs */, 8 /*max outputs */, sizeof(output_type)))
    {
    	
        p.initialize(num_ant_,samp_rate,fft_size_,num_ofdm_per_frame_,vht_en,num_sc_pha_noise);
        freq_frame=new_vec_2(cpx,p.num_ant,p.fft_size*p.num_payload_ofdm_per_frame,gr_complex(0,0));
    	set_output_multiple(p.fft_size*p.num_payload_ofdm_per_frame);
    }

    /*
     * Our virtual destructor.
     */
    tx_freq_frame_impl::~tx_freq_frame_impl()
    {
    }

    void
    tx_freq_frame_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
    	ninput_items_required[0]=p.payload_sc.size()*p.num_ant*p.num_payload_ofdm_per_frame;
    }

    int
    tx_freq_frame_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      //set input and output
      auto in = static_cast<const input_type*>(input_items[0]);
      output_type* out[8];
      
      
      for(int n=0;n<p.num_ant;n++)
      	out[n] = (output_type *) output_items[n];
      	
      	
      // ensure it has enough data for one ofdm symbol 
	if (ninput_items[0] < p.payload_sc.size()*p.num_ant*p.num_payload_ofdm_per_frame) {
		cout<<"freq_frame: input data is not sufficient!"<<endl;
		return 0;
	}

	// ensure it has enough data for one ofdm symbol 
	if (noutput_items < p.fft_size*p.num_payload_ofdm_per_frame) {
		cout<<"freq_frame: output buffer is not sufficient!"<<endl;
		return 0;
	}
		
			
      //insert freq_pilot
      int num_payload=0;
      for(int i=0;i<p.num_ant;i++)
      	for(int j=0;j<p.num_payload_ofdm_per_frame;j++){
      		//insert freq_pilot
      		for(int k=0;k<p.pilot_sc.size();k++)
      			freq_frame[i][j*p.fft_size+p.pilot_sc[k]]=p.pilot;
      		
      	
      		//insert payload			
      		for(int l=0;l<p.payload_sc.size();l++)
      			freq_frame[i][j*p.fft_size+p.payload_sc[l]]=in[num_payload++];
      }
      
      
      		
	

      for(int i=0;i<p.num_ant;i++)
      	memcpy(out[i],&freq_frame[i][0],p.fft_size*p.num_payload_ofdm_per_frame*sizeof(gr_complex));
	
	
	
      consume_each(p.payload_sc.size()*p.num_ant*p.num_payload_ofdm_per_frame);
      return p.fft_size*p.num_payload_ofdm_per_frame;
      //return noutput_items;
    }

  } /* namespace mmwave_bw */
} /* namespace gr */
