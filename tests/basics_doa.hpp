#pragma once
#include <Eigen/Dense>
#include <complex>
#include <vector>
#include <cmath>

using namespace Eigen;
using namespace std;

// --- 1. Vecteur directeur (Steering Vector) ---
// theta: Angle d'arrivée en radians
// d_lambda: Espacement des antennes en fonction de la longueur d'onde
VectorXcf compute_steering_vector(double theta, int num_antennas, double d_lambda) {
    VectorXcf a(num_antennas);
    for (int i = 0; i < num_antennas; ++i) {
        // Formule classique d'un réseau linéaire : exp(-j * 2 * pi * i * d_lambda * sin(theta))
        complex<double> exponent(0, -2.0 * M_PI * i * d_lambda * sin(theta));
        a(i) = exp(exponent);
    }
    return a;
}

// --- 2. Filtre NLMS (Mise à jour des poids) ---
// w: Poids actuels
// x: Vecteur d'entrée (échantillons)
// d: Signal désiré
// mu: Pas d'adaptation
void update_nlms(VectorXcf& w, const VectorXcf& x, complex<float> d, double mu) {
    complex<float> y = w.dot(x);       // Sortie du filtre
    complex<float> error = d - y;      // Erreur
    double norm_x = x.squaredNorm();    // Puissance du signal d'entrée
    
    if (norm_x > 1e-6) {
        // w(n+1) = w(n) + mu * conj(x) * error / ||x||^2
        w += (mu / norm_x) * x.conjugate() * error;
    }
}

// --- 3. Angle Unwrapping ---
// Permet d'éviter les sauts brutaux entre -pi et pi
double unwrap_angle(double current_angle, double previous_angle) {
    double delta = current_angle - previous_angle;
    if (delta > M_PI) current_angle -= 2 * M_PI;
    else if (delta < -M_PI) current_angle += 2 * M_PI;
    return current_angle;
}