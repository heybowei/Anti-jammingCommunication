/* -*- c++ -*- */
/*
 * Copyright 2023 bowei.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MMWAVE_BW_RX_FFT_IMPL_H
#define INCLUDED_MMWAVE_BW_RX_FFT_IMPL_H

#include <gnuradio/mmwave_bw/rx_fft.h>
#include <armadillo>
#include "para.h"

namespace gr {
  namespace mmwave_bw {

    class rx_fft_impl : public rx_fft
    {
     private:
      para p;

     public:
      rx_fft_impl(int fft_size_,int num_ofdm_per_frame);
      ~rx_fft_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace mmwave_bw
} // namespace gr

#endif /* INCLUDED_MMWAVE_BW_RX_FFT_IMPL_H */
