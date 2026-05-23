/* -*- c++ -*- */
/*
 * Copyright 2026 HAMMADI LYES/KEBIECHE FARES.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "adaptive_doa_ula_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace rfst_ula {

#pragma message("set the following appropriately and remove this warning")
using input_type = float;
#pragma message("set the following appropriately and remove this warning")
using output_type = float;
adaptive_doa_ula::sptr adaptive_doa_ula::make(float mu, float eta, float initial_theta)
{
    return gnuradio::make_block_sptr<adaptive_doa_ula_impl>(mu, eta, initial_theta);
}


/*
 * The private constructor
 */
adaptive_doa_ula_impl::adaptive_doa_ula_impl(float mu, float eta, float initial_theta)
    : gr::sync_block("adaptive_doa_ula",
                     gr::io_signature::make(
                         1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                     gr::io_signature::make(
                         1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
{
}

/*
 * Our virtual destructor.
 */
adaptive_doa_ula_impl::~adaptive_doa_ula_impl() {}

int adaptive_doa_ula_impl::work(int noutput_items,
                                gr_vector_const_void_star& input_items,
                                gr_vector_void_star& output_items)
{
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);

#pragma message("Implement the signal processing in your block and remove this warning")
    // Do <+signal processing+>

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace rfst_ula */
} /* namespace gr */
