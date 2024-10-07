/* -*- c++ -*- */
/*
 * Copyright 2023 bowei.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MMWAVE_BW_RX_DEQAM_IMPL_H
#define INCLUDED_MMWAVE_BW_RX_DEQAM_IMPL_H

#include <gnuradio/mmwave_bw/rx_deqam.h>
#include "para.h"

namespace gr {
  namespace mmwave_bw {

    class rx_deqam_impl : public rx_deqam
    {
     private:
	int qam_order;
	
		
	vec_1(cpx) bpsk;
	vec_1(cpx) qpsk;
	vec_1(cpx) qam16;

     public:
      rx_deqam_impl(int qam_order);
      ~rx_deqam_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace mmwave_bw
} // namespace gr

#endif /* INCLUDED_MMWAVE_BW_RX_DEQAM_IMPL_H */
