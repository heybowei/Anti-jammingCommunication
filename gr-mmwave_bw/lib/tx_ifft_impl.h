/* -*- c++ -*- */
/*
 * Copyright 2023 bowei.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MMWAVE_BW_TX_IFFT_IMPL_H
#define INCLUDED_MMWAVE_BW_TX_IFFT_IMPL_H

#include <gnuradio/mmwave_bw/tx_ifft.h>
#include "para.h"
#include <armadillo>


namespace gr {
  namespace mmwave_bw {

    class tx_ifft_impl : public tx_ifft
    {
     private:
      int fft_size;

     public:
      tx_ifft_impl(int fft_size_);
      ~tx_ifft_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace mmwave_bw
} // namespace gr

#endif /* INCLUDED_MMWAVE_BW_TX_IFFT_IMPL_H */
