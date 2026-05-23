#ifndef INCLUDED_RFST_ULA_ADAPTIVE_DOA_ULA_IMPL_H
#define INCLUDED_RFST_ULA_ADAPTIVE_DOA_ULA_IMPL_H

#include <gnuradio/rfst_ula/adaptive_doa_ula.h>
#include <gnuradio/thread/thread.h>
#include <array>
#include <chrono>
#include <cmath>

namespace gr {
namespace rfst_ula {

class adaptive_doa_ula_impl : public adaptive_doa_ula
{
private:
    // ─── Constantes ULA (M=5 fixe, d=λ/2) ───────────────────────────
    static constexpr int    M    = 5;
    static constexpr double KD   = M_PI;          // k*d = 2π * 0.5
    static constexpr double KMAT = 10.0;           // Σ(x²) - (Σx)²/M = 10
    // Positions antennes : [-2, -1, 0, 1, 2]
    static constexpr std::array<double,5> X_POS = {-2.,-1.,0.,1.,2.};
    // sin(±89°) pour le clipping
    static constexpr double SIN_MAX = 0.99984769515639127;

    // ─── Paramètres algo ─────────────────────────────────────────────
    float  d_mu;
    float  d_eta;
    gr::thread::mutex d_mutex;   // protection set_mu/set_eta

    // ─── État récursif (persistant entre les blocs) ───────────────────
    double   d_k_spa;                    // fréquence spatiale courante
    std::array<gr_complex, M>   d_G;    // vecteur directeur
    std::array<gr_complex, M-1> d_W;    // poids LMS

    // ─── Télémétrie ───────────────────────────────────────────────────
    uint64_t d_total_samples;
    uint64_t d_samples_since_report;
    std::chrono::steady_clock::time_point d_last_report;

    // ─── Utilitaires inlined ─────────────────────────────────────────
    // Calcule exp(j*phi) en float
    inline gr_complex expjf(double phi) const noexcept {
        return gr_complex(static_cast<float>(std::cos(phi)),
                          static_cast<float>(std::sin(phi)));
    }

    // Reconstruit G[0..4] à partir de ek1=exp(j*k), ek2=exp(j*2k)
    // Économise 3 sin/cos supplémentaires par rapport à 5 appels naïfs
    inline void rebuild_G(const gr_complex& ek1,
                          const gr_complex& ek2,
                          std::array<gr_complex,M>& G) const noexcept {
        G[0] = std::conj(ek2);          // exp(-j*2*k)
        G[1] = std::conj(ek1);          // exp(-j*k)
        G[2] = gr_complex(1.0f, 0.0f);  // exp(0)
        G[3] = ek1;                     // exp(+j*k)
        G[4] = ek2;                     // exp(+j*2*k)
    }

public:
    adaptive_doa_ula_impl(float mu, float eta, float initial_theta);
    ~adaptive_doa_ula_impl() override = default;

    void set_mu(float mu)   override;
    void set_eta(float eta) override;
    float get_theta()       const override;

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star&       output_items) override;
};

} // namespace rfst_ula
} // namespace gr

#endif