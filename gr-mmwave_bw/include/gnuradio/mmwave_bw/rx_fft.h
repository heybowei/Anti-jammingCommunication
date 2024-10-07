/* -*- c++ -*- */
/*
 * Copyright 2023 bowei.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MMWAVE_BW_RX_FFT_H
#define INCLUDED_MMWAVE_BW_RX_FFT_H

#include <gnuradio/mmwave_bw/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace mmwave_bw {

    /*!
     * \brief <+description of block+>
     * \ingroup mmwave_bw
     *
     */
    class MMWAVE_BW_API rx_fft : virtual public gr::block
    {
     public:
      typedef std::shared_ptr<rx_fft> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of mmwave_bw::rx_fft.
       *
       * To avoid accidental use of raw pointers, mmwave_bw::rx_fft's
       * constructor is in a private implementation
       * class. mmwave_bw::rx_fft::make is the public interface for
       * creating new instances.
       */
      static sptr make(int fft_size_,int num_ofdm_per_frame);
    };

  } // namespace mmwave_bw
} // namespace gr

#endif /* INCLUDED_MMWAVE_BW_RX_FFT_H */
