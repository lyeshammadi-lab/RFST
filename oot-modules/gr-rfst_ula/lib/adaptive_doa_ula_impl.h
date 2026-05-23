#ifndef INCLUDED_RFST_ULA_ADAPTIVE_DOA_ULA_IMPL_H
#define INCLUDED_RFST_ULA_ADAPTIVE_DOA_ULA_IMPL_H

#include <gnuradio/rfst_ula/adaptive_doa_ula.h>
#include <array>
#include <complex>
#include <chrono>
#include <gnuradio/thread/thread.h>

namespace gr {
namespace rfst_ula {

using gr_complex = std::complex<float>;

class adaptive_doa_ula_impl : public adaptive_doa_ula
{
private:
    static constexpr int M = 5;
    static constexpr double KD = M_PI;
    static constexpr double KMAT = 10.0;
    static constexpr double SIN_MAX = 0.9998476951563913;

    static constexpr std::array<double, 5> X_POS = {-2.0, -1.0, 0.0, 1.0, 2.0};

    float d_mu;
    float d_eta;
    double d_k_spa;

    std::array<gr_complex, M> d_G;
    std::array<gr_complex, M-1> d_W;

    mutable gr::thread::mutex d_mutex;

    uint64_t d_total_samples;
    uint64_t d_samples_since_report;
    std::chrono::steady_clock::time_point d_last_report;

    inline gr_complex expjf(double phi) const noexcept {
        return gr_complex(static_cast<float>(std::cos(phi)),
                          static_cast<float>(std::sin(phi)));
    }

    inline void rebuild_G(const gr_complex& ek1, const gr_complex& ek2,
                          std::array<gr_complex, M>& G) const noexcept {
        G[0] = std::conj(ek2);
        G[1] = std::conj(ek1);
        G[2] = gr_complex(1.0f, 0.0f);
        G[3] = ek1;
        G[4] = ek2;
    }

public:
    adaptive_doa_ula_impl(float mu, float eta, float initial_theta);
    ~adaptive_doa_ula_impl() override = default;

    void set_mu(float mu) override;
    void set_eta(float eta) override;
    float get_theta() const override;

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items) override;
};

} // namespace rfst_ula
} // namespace gr

#endif
