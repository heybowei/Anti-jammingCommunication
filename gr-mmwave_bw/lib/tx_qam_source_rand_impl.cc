/* -*- c++ -*- */
/*
 * Copyright 2023 bowei.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "tx_qam_source_rand_impl.h"
#include <random>

namespace gr {
  namespace mmwave_bw {

    using output_type = gr_complex;
    tx_qam_source_rand::sptr
    tx_qam_source_rand::make(int num_of_qam)
    {
      return gnuradio::make_block_sptr<tx_qam_source_rand_impl>(
        num_of_qam);
    }


    /*
     * The private constructor
     */
    tx_qam_source_rand_impl::tx_qam_source_rand_impl(int num_of_qam)
      : gr::sync_block("tx_qam_source_rand",
              gr::io_signature::make(0 /* min inputs */, 0 /* max inputs */, 0),
              gr::io_signature::make(1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
    {
	    numqam=num_of_qam;
	          qam={
		    {-1,0},{1,0},
						//bpsk
		    {-0.7071,-0.7071},{0.7071,-0.7071},
		    {-0.7071,0.7071},{0.7071,0.7071},
				  		//qpsk
	    	{-0.3162, -0.3162}, {0.3162, -0.3162}, {-0.9487, -0.3162}, {0.9487, -0.3162}, 
		{-0.3162, 0.3162}, {0.3162, 0.3162}, {-0.9487, 0.3162}, {0.9487, 0.3162},
	       	{-0.3162, -0.9487}, {0.3162, -0.9487}, {-0.9487, -0.9487},{0.9487,-0.9487},
	       	{-0.3162, 0.9487}, {0.3162, 0.9487}, {-0.9487, 0.9487}, {0.9487, 0.9487}
		    				//qam16
	    };
    }

    /*
     * Our virtual destructor.
     */
    tx_qam_source_rand_impl::~tx_qam_source_rand_impl()
    {
    }

    int
    tx_qam_source_rand_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      auto out = static_cast<output_type*>(output_items[0]);

	int header[5]={0,0,2,0,6};
	int times[5]={0,2,4,0,16};
      double rand_number;
      std::default_random_engine generator;
      std::uniform_real_distribution<double> distribution(0.0,1.0);
      for(int i=0;i<noutput_items;i++){
	rand_number=distribution(generator);
	if(rand_number>1) 
		rand_number=1.0;
	if(rand_number<0)
		rand_number=0.0;
	out[i]=qam[header[numqam]+int(rand_number*times[numqam])];
      }
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace mmwave_bw */
} /* namespace gr */
