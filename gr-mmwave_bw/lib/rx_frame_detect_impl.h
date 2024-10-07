/* -*- c++ -*- */
/*
 * Copyright 2023 bowei.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MMWAVE_BW_RX_FRAME_DETECT_IMPL_H
#define INCLUDED_MMWAVE_BW_RX_FRAME_DETECT_IMPL_H

#include <gnuradio/mmwave_bw/rx_frame_detect.h>
#include "para.h"

namespace gr {
  namespace mmwave_bw {
  
  /*
    struct frame_info{
    	int frame_num;
    	vec_1(int) start_index;
    	vec_1(float) max_val;
    };*/

    class rx_frame_detect_impl : public rx_frame_detect
    {
     private:
       vec_1(float) c_num;
       float noise_ampl_thres;
       int search_len=100;
       int signal_start=0;
       gr_complex* in[8];
       gr_complex* out[8];
       int start_index;
       float max_val;
       int project_en;
       int freq_offset_en;
       float ltf_corr_thres;
       float freq_offset;
       para p;
       
     public:
      rx_frame_detect_impl(int num_ant, int num_stm, float samp_rate, int fft_size, int num_ofdm_per_frame, int vht_en, int project_en, int search_step_size, float noise_ampl_thres, float ltf_corr_thres, int freq_offset_en, int num_sc_pha_noise);
      ~rx_frame_detect_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
      float find_preamble(int n);
      void freq_offset_estimation(int start_index, int start);
    };

  } // namespace mmwave_bw
} // namespace gr

#endif /* INCLUDED_MMWAVE_BW_RX_FRAME_DETECT_IMPL_H */
