/* -*- c++ -*- */
/*
 * Copyright 2023 bowei.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MMWAVE_BW_RX_FRAME_EQUALIZATION_CSI_IMPL_H
#define INCLUDED_MMWAVE_BW_RX_FRAME_EQUALIZATION_CSI_IMPL_H

#include <gnuradio/mmwave_bw/rx_frame_equalization_csi.h>
#include "para.h"

namespace gr {
  namespace mmwave_bw {

    class rx_frame_equalization_csi_impl : public rx_frame_equalization_csi
    {
     private:
      para p;
      int dec_alg;
      vec_2(cpx) h;
      vec_3(cpx) h_mimo;
      vec_3(cpx) h_mimo_inver;
      vec_1(cpx) data;
       gr_complex* in[8];
       gr_complex* out[9];
     public:
      rx_frame_equalization_csi_impl(int num_ant, int num_stm, float samp_rate, int fft_size, int num_ofdm_per_frame, int dec_alg, int step_size, int vht_en, int num_sc_pha_noise);
      ~rx_frame_equalization_csi_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
      void lft_based_mrc_equalization();
      void phase_detect();
      void mimo_zero_forcing();

    };

  } // namespace mmwave_bw
} // namespace gr

#endif /* INCLUDED_MMWAVE_BW_RX_FRAME_EQUALIZATION_CSI_IMPL_H */
