/* -*- c++ -*- */
/*
 * Copyright 2023 bowei.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MMWAVE_BW_RX_FRAME_EQUALIZATION_CSI_H
#define INCLUDED_MMWAVE_BW_RX_FRAME_EQUALIZATION_CSI_H

#include <gnuradio/mmwave_bw/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace mmwave_bw {

    /*!
     * \brief <+description of block+>
     * \ingroup mmwave_bw
     *
     */
    class MMWAVE_BW_API rx_frame_equalization_csi : virtual public gr::block
    {
     public:
      typedef std::shared_ptr<rx_frame_equalization_csi> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of mmwave_bw::rx_frame_equalization_csi.
       *
       * To avoid accidental use of raw pointers, mmwave_bw::rx_frame_equalization_csi's
       * constructor is in a private implementation
       * class. mmwave_bw::rx_frame_equalization_csi::make is the public interface for
       * creating new instances.
       */
      static sptr make(int num_ant, int num_stm, float samp_rate, int fft_size, int num_ofdm_per_frame, int dec_alg, int step_size, int vht_en, int num_sc_pha_noise);
    };

  } // namespace mmwave_bw
} // namespace gr

#endif /* INCLUDED_MMWAVE_BW_RX_FRAME_EQUALIZATION_CSI_H */
