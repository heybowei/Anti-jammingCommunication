/* -*- c++ -*- */
/*
 * Copyright 2023 bowei.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "tx_ifft_impl.h"


namespace gr {
  namespace mmwave_bw {

    using input_type = gr_complex;
    using output_type = gr_complex;
    tx_ifft::sptr
    tx_ifft::make(int fft_size_)
    {
      return gnuradio::make_block_sptr<tx_ifft_impl>(
        fft_size_);
    }


    /*
     * The private constructor
     */
    tx_ifft_impl::tx_ifft_impl(int fft_size_)
      : gr::block("tx_ifft",
              gr::io_signature::make(1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
              gr::io_signature::make(1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
    {

    	fft_size=fft_size_;
    	set_output_multiple(fft_size);
    }

    /*
     * Our virtual destructor.
     */
    tx_ifft_impl::~tx_ifft_impl()
    {
    }

    void
    tx_ifft_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = fft_size;
    }

    int
    tx_ifft_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      //auto in = static_cast<const input_type*>(input_items[0]);
      //auto out = static_cast<output_type*>(output_items[0]);
	gr_complex* in = (gr_complex*) input_items[0];
	gr_complex* out = (gr_complex*) output_items[0];
	
	if (ninput_items[0] < fft_size) 
		return 0;
      
        arma::cx_vec sig_ff(fft_size);
      	
	for (int j = 0; j < fft_size; j++)
		sig_ff(j) = in[j];

	arma::cx_vec sig_tt=ifft(sig_ff);

	for (int j = 0; j < fft_size; j++)
		out[j] = sig_tt[j];



      
      consume_each(fft_size);

      // Tell runtime system how many output items we produced.
      return fft_size;
    }

  } /* namespace mmwave_bw */
} /* namespace gr */
