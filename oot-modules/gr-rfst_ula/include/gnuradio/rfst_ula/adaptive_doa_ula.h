/* -*- c++ -*- */
/*
 * Copyright 2026 HAMMADI LYES/KEBIECHE FARES.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_RFST_ULA_ADAPTIVE_DOA_ULA_H
#define INCLUDED_RFST_ULA_ADAPTIVE_DOA_ULA_H

#include <gnuradio/rfst_ula/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace rfst_ula {

/*!
 * \brief <+description of block+>
 * \ingroup rfst_ula
 *
 */
class RFST_ULA_API adaptive_doa_ula : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<adaptive_doa_ula> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of rfst_ula::adaptive_doa_ula.
     *
     * To avoid accidental use of raw pointers, rfst_ula::adaptive_doa_ula's
     * constructor is in a private implementation
     * class. rfst_ula::adaptive_doa_ula::make is the public interface for
     * creating new instances.
     */
    static sptr make(float mu = 0.05f, float eta = 1e-5f, float initial_theta = 0.0f);
};

} // namespace rfst_ula
} // namespace gr

#endif /* INCLUDED_RFST_ULA_ADAPTIVE_DOA_ULA_H */
