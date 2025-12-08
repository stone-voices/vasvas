#pragma once

#include <cmath>
#include <complex>
#include <numbers>

#include "fastsqrt.h"

//=============================================================================
class BandShifter
{
public:
    BandShifter() = default;

    BandShifter(float center, float sample_rate, float bw)
    {
        constexpr auto pi = std::numbers::pi_v<double>;
        constexpr auto j = std::complex<double>(0, 1);

        const auto w0 = pi * bw / sample_rate;
        const auto cos_w0 = std::cos(w0);
        const auto sin_w0 = std::sin(w0);
        const auto sqrt_2 = std::sqrt(2.0);
        const auto a0 = (1 + sqrt_2 * sin_w0 / 2);
        const auto g = (1 - cos_w0) / (2 * a0);

        const auto w1 = 2 * pi * center / sample_rate;
        const auto e1 = std::exp(j * w1);
        const auto e2 = std::exp(j * w1 * 2.0);

        const auto d0 = g;
        const auto d1 = e1 * 2.0 * g;
        const auto d2 = e2 * g;
        const auto c1 = e1 * (-2 * cos_w0) / a0;
        const auto c2 = e2 * (1 - sqrt_2 * sin_w0 / 2) / a0;

        _d0 = d0;
        _d1 = std::complex<float>(d1.real(), d1.imag());
        _d2 = std::complex<float>(d2.real(), d2.imag());
        _c1 = std::complex<float>(c1.real(), c1.imag());
        _c2 = std::complex<float>(c2.real(), c2.imag());
    }

    void update(float sample)
    {
        update_filter(sample);
        update_up1();
    }

    float up1() const {
        return _up1;
    }

private:
    // Prototype filter is LPF from "Cookbook formulae for audio EQ biquad
    // filter coefficients", a.k.a. "Audio EQ Cookbook",
    // by Robert Bristow-Johnson
    // https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html
    //
    // Transformed as described in Section 3.1 of "Complex Band-Pass Filters
    // for Analytic Signal Generation and Their Application" by Andrew J. Noga
    // https://apps.dtic.mil/sti/tr/pdf/ADA395963.pdf

    void update_filter(float sample)
    {
        const auto prev_y = _y;
        _y = _s2 + _d0*sample;
        _s2 = _s1 + _d1*sample - _c1*_y;
        _s1 = _d2*sample - _c2*_y;
    }

    // Octave shifts are performed via phase scaling described in "Real-Time
    // Polyphonic Octave Doubling for the Guitar" by Etienne Thuillier
    // https://core.ac.uk/download/pdf/80719011.pdf
    //
    // in = complex input signal
    // out = scaled complex output signal
    // g = scaling factor
    //
    // out = in * (in / |in|)^(g - 1)
    //
    // Note that for octave down (g = 1/2), it is necessary to detect phase
    // transitions in order to set the sign of the output signal.

    void update_up1()
    {
        const auto a = _y.real();
        const auto b = _y.imag();
        _up1 = (a*a - b*b) * fastInvSqrt(a*a + b*b);
    }


    float _d0 = 0;
    std::complex<float> _d1;
    std::complex<float> _d2;
    std::complex<float> _c1;
    std::complex<float> _c2;

    std::complex<float> _s1;
    std::complex<float> _s2;

    std::complex<float> _y;
    float _up1 = 0;
    std::complex<float> _down1;

};
