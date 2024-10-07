/* -*- c++ -*- */
/*
 * Copyright 2023 bowei.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MMWAVE_BW_TX_TIME_FRAME_IMPL_H
#define INCLUDED_MMWAVE_BW_TX_TIME_FRAME_IMPL_H

#include <gnuradio/mmwave_bw/tx_time_frame.h>
#include "para.h"

namespace gr {
  namespace mmwave_bw {

    class tx_time_frame_impl : public tx_time_frame
    {
     private:
      para p;

     public:
      tx_time_frame_impl(int num_ant_,float samp_rate_,int fft_size_,int num_ofdm_per_frame_,int vht_en_,int num_sc_pha_noise_);
      ~tx_time_frame_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace mmwave_bw
} // namespace gr

#endif /* INCLUDED_MMWAVE_BW_TX_TIME_FRAME_IMPL_H */
