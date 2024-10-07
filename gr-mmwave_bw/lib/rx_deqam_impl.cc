/* -*- c++ -*- */
/*
 * Copyright 2023 bowei.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "rx_deqam_impl.h"

namespace gr {
  namespace mmwave_bw {

    using input_type = gr_complex;
    using output_type = char;
    rx_deqam::sptr
    rx_deqam::make(int qam_order)
    {
      return gnuradio::make_block_sptr<rx_deqam_impl>(
        qam_order);
    }


    /*
     * The private constructor
     */
    rx_deqam_impl::rx_deqam_impl(int qam_order_)
      : gr::block("rx_deqam",
              gr::io_signature::make(1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
              gr::io_signature::make(1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
    {
    		qam_order=qam_order_;
		bpsk = {{-1, 0}, {1, 0}};
		qpsk = {{-0.7071, -0.7071}, {0.7071, -0.7071},  {-0.7071, 0.7071}, {0.7071, 0.7071}};
		qam16 = {
				{-0.3162, -0.3162}, {0.3162, -0.3162}, {-0.9487, -0.3162}, {0.9487, -0.3162},
				{-0.3162, 0.3162}, {0.3162, 0.3162}, {-0.9487, 0.3162}, {0.9487, 0.3162},
				{-0.3162, -0.9487}, {0.3162, -0.9487}, {-0.9487, -0.9487}, {0.9487, -0.9487},
				{-0.3162, 0.9487}, {0.3162, 0.9487}, {-0.9487, 0.9487}, {0.9487, 0.9487}
				};
	
		set_output_multiple(200);
    }

    /*
     * Our virtual destructor.
     */
    rx_deqam_impl::~rx_deqam_impl()
    {
    }

    void
    rx_deqam_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
		if (qam_order == 1) { // BPSK
			ninput_items_required[0] = 8*noutput_items;
		}
		else if (qam_order == 2) {  // QPSK
			ninput_items_required[0] = 4*noutput_items;
		}
		else if (qam_order == 4) {  // 16QPSK
			ninput_items_required[0] = 2*noutput_items;
		}
		else {
			cout<<"The modulation scheme is not supported!"<<endl;
		}
    }

    int
    rx_deqam_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
		int num_samp;
		int num_byte;
		const gr_complex * in = (const gr_complex *) input_items[0];
		unsigned char *out = (unsigned char *) output_items[0];
		unsigned char c;
	
	
		if (qam_order == 1) { // BPSK
			num_samp = ninput_items[0] < 8*noutput_items ? ninput_items[0] : 8*noutput_items;
			num_byte = num_samp/8;
			for (int i = 0; i < num_byte; i++) {
				c = 0x00;
				for (int k = 0; k < 8; k++) {
					c += (in[i*8+k].real() > 0) << k;
				}
				out[i] = c ^ 0b01010101;
			}
		}
		else if (qam_order == 2) {  // QPSK
			num_samp = ninput_items[0] < 4*noutput_items ? ninput_items[0] : 4*noutput_items;
			num_byte = num_samp/4;
			for (int i = 0; i < num_byte; i++) {
				c = 0x00;
				for (int k = 0; k < 4; k++) {
					c += (in[i*4+k].real() > 0) << 2*k;
					c += (in[i*4+k].imag() > 0) << 2*k+1;
				}
				out[i] = c ^ 0b00011011;
			}
				
		}
		else if (qam_order == 4) {  // 16QPSK
			num_samp = ninput_items[0] < 2*noutput_items ? ninput_items[0] : 2*noutput_items;
			num_byte = num_samp/2;
			for (int i = 0; i < num_byte; i++) {
				c = 0x00;
				for (int k = 0; k < 2; k++) {
					c += (in[i*2+k].real() > 0) << 4*k;
					c += (abs(in[i*2+k].real()) > 0.63245) << 4*k+1;					
					c += (in[i*2+k].imag() > 0) << 4*k+2;
					c += (abs(in[i*2+k].imag()) > 0.63245) << 4*k+3;
				}
				out[i] = c ^ 0b00000101;
			}
		}
		else {
			cout<<"Demodulation: the modulation scheme is not supported!"<<endl;
		}	
		
		
		consume_each (num_samp);
		return num_byte;
    }

  } /* namespace mmwave_bw */
} /* namespace gr */
