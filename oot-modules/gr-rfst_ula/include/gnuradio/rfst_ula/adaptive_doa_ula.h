#ifndef INCLUDED_RFST_ULA_ADAPTIVE_DOA_ULA_H
#define INCLUDED_RFST_ULA_ADAPTIVE_DOA_ULA_H

#include <gnuradio/rfst_ula/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace rfst_ula {

class RFST_ULA_API adaptive_doa_ula : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<adaptive_doa_ula> sptr;

    static sptr make(float mu = 0.05f, float eta = 1e-5f, float initial_theta = 0.0f);

    virtual void set_mu(float mu) = 0;
    virtual void set_eta(float eta) = 0;
    virtual float get_theta() const = 0;
};

} // namespace rfst_ula
} // namespace gr

#endif
