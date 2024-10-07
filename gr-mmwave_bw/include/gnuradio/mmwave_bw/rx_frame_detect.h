/* -*- c++ -*- */
/*
 * Copyright 2023 bowei.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MMWAVE_BW_RX_FRAME_DETECT_H
#define INCLUDED_MMWAVE_BW_RX_FRAME_DETECT_H

#include <gnuradio/mmwave_bw/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace mmwave_bw {

    /*!
     * \brief <+description of block+>
     * \ingroup mmwave_bw
     *
     */
    class MMWAVE_BW_API rx_frame_detect : virtual public gr::block
    {
     public:
      typedef std::shared_ptr<rx_frame_detect> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of mmwave_bw::rx_frame_detect.
       *
       * To avoid accidental use of raw pointers, mmwave_bw::rx_frame_detect's
       * constructor is in a private implementation
       * class. mmwave_bw::rx_frame_detect::make is the public interface for
       * creating new instances.
       */
      static sptr make(int num_ant, int num_stm, float samp_rate, int fft_size, int num_ofdm_per_frame, int vht_en, int project_en, int search_step_size, float noise_ampl_thres, float ltf_corr_thres, int freq_offset_en, int num_sc_pha_noise);
    };

  } // namespace mmwave_bw
} // namespace gr

#endif /* INCLUDED_MMWAVE_BW_RX_FRAME_DETECT_H */
