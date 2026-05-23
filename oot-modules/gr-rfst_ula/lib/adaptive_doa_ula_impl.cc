#include "adaptive_doa_ula_impl.h"
#include <gnuradio/io_signature.h>
#include <iostream>
#include <iomanip>

namespace gr {
namespace rfst_ula {

// ─── Définitions des membres statiques (C++14) ───────────────────────────────
constexpr std::array<double, 5> adaptive_doa_ula_impl::X_POS;

// ─── Factory ─────────────────────────────────────────────────────────────────
adaptive_doa_ula::sptr
adaptive_doa_ula::make(float mu, float eta, float initial_theta)
{
    return std::make_shared<adaptive_doa_ula_impl>(mu, eta, initial_theta);
}

// ─── Constructeur ────────────────────────────────────────────────────────────
adaptive_doa_ula_impl::adaptive_doa_ula_impl(float mu,
                                              float eta,
                                              float initial_theta)
    : gr::sync_block(
          "adaptive_doa_ula",
          gr::io_signature::make(5, 5, sizeof(gr_complex)),
          gr::io_signature::makev(
              2, 2,
              std::vector<int>{ static_cast<int>(sizeof(float)),
                                static_cast<int>(sizeof(gr_complex) * M) }))
    , d_mu(mu)
    , d_eta(eta)
    , d_total_samples(0)
    , d_samples_since_report(0)
    , d_last_report(std::chrono::steady_clock::now())
{
    d_k_spa = KD * std::sin(static_cast<double>(initial_theta) * M_PI / 180.0);

    double ck = std::cos(d_k_spa), sk = std::sin(d_k_spa);
    gr_complex ek1(static_cast<float>(ck), static_cast<float>(sk));
    gr_complex ek2(static_cast<float>(ck * ck - sk * sk),
                   static_cast<float>(2.0 * ck * sk));
    rebuild_G(ek1, ek2, d_G);

    d_W.fill(gr_complex(0.0f, 0.0f));
    set_output_multiple(1024);
}

// ─── Destructeur ─────────────────────────────────────────────────────────────
adaptive_doa_ula_impl::~adaptive_doa_ula_impl() = default;

// ─── Setters thread-safe ─────────────────────────────────────────────────────
void adaptive_doa_ula_impl::set_mu(float mu)
{
    gr::thread::scoped_lock lock(d_mutex);
    d_mu = mu;
}

void adaptive_doa_ula_impl::set_eta(float eta)
{
    gr::thread::scoped_lock lock(d_mutex);
    d_eta = eta;
}

float adaptive_doa_ula_impl::get_theta() const
{
    double val = d_k_spa / KD;
    if (val > SIN_MAX)
        val = SIN_MAX;
    else if (val < -SIN_MAX)
        val = -SIN_MAX;
    return static_cast<float>(-std::asin(val) * (180.0 / M_PI));
}

// ─── work() ──────────────────────────────────────────────────────────────────
int adaptive_doa_ula_impl::work(int noutput_items,
                                 gr_vector_const_void_star& input_items,
                                 gr_vector_void_star& output_items)
{
    const gr_complex* in[M];
    for (int i = 0; i < M; ++i)
        in[i] = static_cast<const gr_complex*>(input_items[i]);

    float* out_theta = static_cast<float*>(output_items[0]);
    gr_complex* out_G = static_cast<gr_complex*>(output_items[1]);

    double k_spa = d_k_spa;
    float mu = d_mu;
    float eta = d_eta;

    gr_complex G[M], W[M - 1];
    for (int i = 0; i < M; ++i)
        G[i] = d_G[i];
    for (int i = 0; i < M - 1; ++i)
        W[i] = d_W[i];

    // ── Télémétrie ────────────────────────────────────────────────────
    d_total_samples += noutput_items;
    d_samples_since_report += noutput_items;
    auto now = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration<double>(now - d_last_report).count();
    if (elapsed >= 1.0) {
        double kSps = static_cast<double>(d_samples_since_report) / elapsed / 1e3;
        std::cout << "[RFST-ULA] Cumulés: " << d_total_samples
                  << " | Vitesse: " << std::fixed << std::setprecision(2)
                  << kSps << " kSps | Bloc: " << noutput_items << " samples";
        d_samples_since_report = 0;
        d_last_report = now;
    }

    // ═════════════════════════════════════════════════════════════════
    //   BOUCLE PRINCIPALE — zéro allocation, pile uniquement
    // ═════════════════════════════════════════════════════════════════
    for (int t = 0; t < noutput_items; ++t) {

        // ── 1. Lecture des 5 antennes ────────────────────────────────
        const gr_complex X[M] = {
            in[0][t], in[1][t], in[2][t], in[3][t], in[4][t]
        };

        // ── 2. Synchronisation Xs[i] = X[i] * conj(G[i]) ───────────
        gr_complex Xs[M];
        for (int i = 0; i < M; ++i)
            Xs[i] = X[i] * std::conj(G[i]);

        // ── 3. Signal désiré y = mean(Xs) ───────────────────────────
        const gr_complex y =
            (Xs[0] + Xs[1] + Xs[2] + Xs[3] + Xs[4]) * (1.0f / M);

        // ── 4. Branches bloquantes Xb[i] = Xs[i+1] - y ─────────────
        gr_complex Xb[M - 1];
        for (int i = 0; i < M - 1; ++i)
            Xb[i] = Xs[i + 1] - y;

        // ── 5. Interférence estimée e = W^H · Xb ────────────────────
        gr_complex e(0.0f, 0.0f);
        for (int i = 0; i < M - 1; ++i)
            e += std::conj(W[i]) * Xb[i];

        // ── 6. Signal propre ─────────────────────────────────────────
        const gr_complex s_hat = y - e;
        const gr_complex csh = std::conj(s_hat);

        // ── 7. Mise à jour LMS des poids W ───────────────────────────
        for (int i = 0; i < M - 1; ++i)
            W[i] -= eta * csh * Xb[i];

        // ── 8. Mise à jour gradient du vecteur directeur G ──────────
        for (int i = 0; i < M; ++i)
            G[i] += mu * (X[i] - G[i] * s_hat) * csh;

        // ── 9. Résidu de phase (OPTIMISÉ : 1 seul sincos) ───────────
        {
            double ck = std::cos(k_spa), sk = std::sin(k_spa);
            const gr_complex ek1(static_cast<float>(ck),
                                  static_cast<float>(sk));
            const gr_complex ek2(static_cast<float>(ck * ck - sk * sk),
                                  static_cast<float>(2.0 * ck * sk));

            const gr_complex ephase[M] = {
                ek2,
                ek1,
                gr_complex(1.0f, 0.0f),
                std::conj(ek1),
                std::conj(ek2)
            };

            double phase_sum = 0.0;
            for (int i = 0; i < M; ++i)
                phase_sum += X_POS[i] *
                             static_cast<double>(std::imag(G[i] * ephase[i]));

            // ── 10. Mise à jour de la fréquence spatiale ─────────────
            k_spa += phase_sum / KMAT;
        }

        // ── 11. Conversion en angle (clip + asin) ────────────────────
        double val_sin = k_spa / KD;
        if (val_sin > SIN_MAX)
            val_sin = SIN_MAX;
        else if (val_sin < -SIN_MAX)
            val_sin = -SIN_MAX;

        const float theta_hat =
            static_cast<float>(std::asin(val_sin) * (180.0 / M_PI));

        // ── 12. Resynchronisation k_spa + reconstruction G ──────────
        k_spa = KD * val_sin;
        {
            double ck = std::cos(k_spa), sk = std::sin(k_spa);
            const gr_complex ek1n(static_cast<float>(ck),
                                   static_cast<float>(sk));
            const gr_complex ek2n(static_cast<float>(ck * ck - sk * sk),
                                   static_cast<float>(2.0 * ck * sk));
            rebuild_G(ek1n, ek2n, *reinterpret_cast<std::array<gr_complex, M>*>(G));
        }

        // ── Écriture des sorties ─
        out_theta[t] = -theta_hat;
        gr_complex* og = out_G + t * M;
        og[0] = -G[0]; og[1] = -G[1]; og[2] = -G[2];
        og[3] = -G[3]; og[4] = -G[4];

    } // fin boucle principale

    // ── Sauvegarde de l'état pour le prochain bloc ────────────────────
    d_k_spa = k_spa;
    for (int i = 0; i < M; ++i)
        d_G[i] = G[i];
    for (int i = 0; i < M - 1; ++i)
        d_W[i] = W[i];

    return noutput_items;
}

} // namespace rfst_ula
} // namespace gr