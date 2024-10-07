/* -*- c++ -*- */
/*
 * Copyright 2023 bowei.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "rx_fft_impl.h"

namespace gr {
  namespace mmwave_bw {

    using input_type = gr_complex;
    using output_type = gr_complex;
    rx_fft::sptr
    rx_fft::make(int fft_size_,int num_ofdm_per_frame)
    {
      return gnuradio::make_block_sptr<rx_fft_impl>(
        fft_size_, num_ofdm_per_frame);
    }


    /*
     * The private constructor
     */
    rx_fft_impl::rx_fft_impl(int fft_size_,int num_ofdm_per_frame)
      : gr::block("rx_fft",
              gr::io_signature::make(1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
              gr::io_signature::make(1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
    {
    	p.initialize(1, -1,fft_size_, num_ofdm_per_frame, 0,0);
    	set_output_multiple(p.num_ofdm_per_frame*p.fft_size);
    }

    /*
     * Our virtual destructor.
     */
    rx_fft_impl::~rx_fft_impl()
    {
    }

    void
    rx_fft_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = p.num_ofdm_per_frame*p.ofdm_size+1;
    }

    int
    rx_fft_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      auto in = static_cast<const input_type*>(input_items[0]);
      auto out = static_cast<output_type*>(output_items[0]);
      vec_1(cpx) sign_buff=new_vec_1(cpx,p.num_ofdm_per_frame*p.ofdm_size,gr_complex(0,0));
      float freq_offset=in[p.num_ofdm_per_frame*p.ofdm_size].real();
      //cout<<freq_offset;
      for(int i=0;i<p.num_ofdm_per_frame*p.ofdm_size;i++)
      	sign_buff[i]=in[i]*gr_complex(cosf(i*freq_offset),sinf(i*freq_offset));
      	
      arma::cx_vec sig_tt(p.fft_size);
      arma::cx_vec sig_ff(p.fft_size);	
      for(int i=0;i<p.num_ofdm_per_frame;i++){
      	for(int j=0;j<p.fft_size;j++)
      		sig_tt[j]=sign_buff[i*p.ofdm_size+p.cp_len+j];
      	sig_ff=fft(sig_tt);
      	
      	for(int j=0;j<p.fft_size;j++)
      		out[i*p.fft_size+j]=sig_ff[j];
      }
      
      consume_each (p.num_ofdm_per_frame*p.ofdm_size+1);

      // Tell runtime system how many output items we produced.
      return p.num_ofdm_per_frame*p.fft_size;
    }

  } /* namespace mmwave_bw */
} /* namespace gr */
