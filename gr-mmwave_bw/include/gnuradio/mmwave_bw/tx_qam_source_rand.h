/* -*- c++ -*- */
/*
 * Copyright 2023 bowei.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MMWAVE_BW_TX_QAM_SOURCE_RAND_H
#define INCLUDED_MMWAVE_BW_TX_QAM_SOURCE_RAND_H

#include <gnuradio/mmwave_bw/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace mmwave_bw {

    /*!
     * \brief <+description of block+>
     * \ingroup mmwave_bw
     *
     */
    class MMWAVE_BW_API tx_qam_source_rand : virtual public gr::sync_block
    {
     public:
      typedef std::shared_ptr<tx_qam_source_rand> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of mmwave_bw::tx_qam_source_rand.
       *
       * To avoid accidental use of raw pointers, mmwave_bw::tx_qam_source_rand's
       * constructor is in a private implementation
       * class. mmwave_bw::tx_qam_source_rand::make is the public interface for
       * creating new instances.
       */
      static sptr make(int num_of_qam=0);
    };

  } // namespace mmwave_bw
} // namespace gr

#endif /* INCLUDED_MMWAVE_BW_TX_QAM_SOURCE_RAND_H */
