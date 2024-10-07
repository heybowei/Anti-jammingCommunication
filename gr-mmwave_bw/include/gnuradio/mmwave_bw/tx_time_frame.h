/* -*- c++ -*- */
/*
 * Copyright 2023 bowei.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MMWAVE_BW_TX_TIME_FRAME_H
#define INCLUDED_MMWAVE_BW_TX_TIME_FRAME_H

#include <gnuradio/mmwave_bw/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace mmwave_bw {

    /*!
     * \brief <+description of block+>
     * \ingroup mmwave_bw
     *
     */
    class MMWAVE_BW_API tx_time_frame : virtual public gr::block
    {
     public:
      typedef std::shared_ptr<tx_time_frame> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of mmwave_bw::tx_time_frame.
       *
       * To avoid accidental use of raw pointers, mmwave_bw::tx_time_frame's
       * constructor is in a private implementation
       * class. mmwave_bw::tx_time_frame::make is the public interface for
       * creating new instances.
       */
      static sptr make(int num_ant_,float samp_rate_,int fft_size_,int num_ofdm_per_frame_,int vht_en_,int num_sc_pha_noise_);
    };

  } // namespace mmwave_bw
} // namespace gr

#endif /* INCLUDED_MMWAVE_BW_TX_TIME_FRAME_H */
