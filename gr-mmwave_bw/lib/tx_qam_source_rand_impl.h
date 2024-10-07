/* -*- c++ -*- */
/*
 * Copyright 2023 bowei.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MMWAVE_BW_TX_QAM_SOURCE_RAND_IMPL_H
#define INCLUDED_MMWAVE_BW_TX_QAM_SOURCE_RAND_IMPL_H
using namespace std;


#include <gnuradio/mmwave_bw/tx_qam_source_rand.h>
#include <vector>
#include <complex>
#include <iostream>
#include "para.h"

namespace gr {
  namespace mmwave_bw {

    class tx_qam_source_rand_impl : public tx_qam_source_rand
    {
     private:
	vec_1(cpx) qam;
       int numqam; //shows the number of qam generator
     public:
      tx_qam_source_rand_impl(int num_of_qam);
      ~tx_qam_source_rand_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace mmwave_bw
} // namespace gr

#endif /* INCLUDED_MMWAVE_BW_TX_QAM_SOURCE_RAND_IMPL_H */
