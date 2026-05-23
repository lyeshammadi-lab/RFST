/* -*- c++ -*- */
/*
 * Copyright 2026 HAMMADI Lyes / KEBIECHE FARES.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_RFST_ULA_ADAPTIVE_DOA_ULA_IMPL_H
#define INCLUDED_RFST_ULA_ADAPTIVE_DOA_ULA_IMPL_H

#include <gnuradio/rfst_ula/adaptive_doa_ula.h>

namespace gr {
namespace rfst_ula {

class adaptive_doa_ula_impl : public adaptive_doa_ula
{
private:
    // Nothing to declare in this block.

public:
    adaptive_doa_ula_impl(float mu, float eta, float initial_theta);
    ~adaptive_doa_ula_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace rfst_ula
} // namespace gr

#endif /* INCLUDED_RFST_ULA_ADAPTIVE_DOA_ULA_IMPL_H */
