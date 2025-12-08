#define _USE_MATH_DEFINES

#include "reverb.h"
#include <cmath>



void TReverb::SetSampleRate(float samplerate)
{
  fSampleRate = samplerate;

  fDelaySamples = fSampleRate / TDelayCell::FsD * (4453 + 3720 + 4217 + 3163);

  fTankDelay1.SetParams(fSampleRate, 4453);
  fTankDelay3.SetParams(fSampleRate, 3720);
  fTankDelay2.SetParams(fSampleRate, 4217);
  fTankDelay4.SetParams(fSampleRate, 3163);

  fAllpass1.SetParams(fSampleRate, 142, 0.998956919f);
  fAllpass2.SetParams(fSampleRate, 107, 0.998956919f);
  fAllpass3.SetParams(fSampleRate, 379, 0.708165646f);
  fAllpass4.SetParams(fSampleRate, 277, 0.708165646f);

  fAllpass5.SetParams(fSampleRate, 1800);
  fAllpass6.SetParams(fSampleRate, 2656);
  fAllpass7.SetParams(fSampleRate, 1511);
  fAllpass8.SetParams(fSampleRate, 2003);
  fAllpass9.SetParams(fSampleRate, 1709);
  fAllpass10.SetParams(fSampleRate, 2411);

  fModAllpass1.SetParams(fSampleRate, 672, 0.6f, 24);
  fModAllpass2.SetParams(fSampleRate, 908, 0.8f, 48);

  fSmootherSize.SetSmoothTime(fSampleRate, 300);
  fSmootherDecay.SetSmoothTime(fSampleRate, 100);
  fSmootherTankDiff.SetSmoothTime(fSampleRate, 100);

  fInpLPF.SetParams(fSampleRate, 10000);
  fInpHPF.SetParams(fSampleRate, 60);
  fTankLPF1.SetParams(fSampleRate, 10000);
  fTankLPF2.SetParams(fSampleRate, 10000);
  fTankHPF1.SetParams(fSampleRate, 20);
  fTankHPF2.SetParams(fSampleRate, 20);

  fA1 = fA2 = fX1 = fX2 = fX3 = fX4 = fX5 = fX6 = fX7 = fX8 = fX9 = fX10 = 0;
  fTankAcc1 = fTankAcc2 = 0;
}

void TReverb::UpdateDecay(float value)
{
  fDecay = value;
  fTankDiff = fDecay + 0.15f;
  if (fTankDiff < 0.15f)
    fTankDiff = 0.15f;
  if (fTankDiff > 0.5f)
    fTankDiff = 0.5f;
}

void TReverb::SetDecay(float value)
{
  auto DecaySamples = value * fSampleRate;
  fDecayVal = powf(10, -3.f * fDelaySamples / DecaySamples);
  if (!fFreeze)
    UpdateDecay(fDecayVal);
}

void TReverb::SetFreeze(bool value)
{
  fFreeze = value;
  UpdateDecay(fFreeze ? 1.0f : fDecayVal);
}

void TReverb::SetSize(float value)
{
  fSize = value;
}



void TReverb::Process(float in, float& outL, float& outR)
{
    float x;
    float tL, tR;
    

    // Smoothing Params
    fSizeSm = fSmootherSize.Process(fSize);
    fDecaySm = fSmootherDecay.Process(fDecay);
    fTankDiffSm = fSmootherTankDiff.Process(fTankDiff);   

    // Input Filters
    x = fInpHPF.Process(fInpLPF.Process(in));

    // Input Diffusers
    x = fAllpass1.Process(x);
    x = fAllpass2.Process(x);
    x = fAllpass3.Process(x);
    x = fAllpass4.Process(x);

    // Tank
    fA1 = fModAllpass1.Process(x + fTankAcc2, fSizeSm);
    fA2 = fModAllpass2.Process(x + fTankAcc1, fSizeSm);

    // Delay lines (1 and 3)
    fX1 = fTankDelay1.Process(fA1);
    fX2 = fTankDelay3.Process(fA2);

    // Tank Filtering (Damping)
    tL = fTankHPF1.Process(fTankLPF1.Process(fX1));
    tR = fTankHPF2.Process(fTankLPF2.Process(fX2));

    // Tank AllPass filters
    fX3 = fAllpass5.Process(tL, fTankDiffSm, fSizeSm);
    fX4 = fAllpass6.Process(tR, fTankDiffSm, fSizeSm);

    // Add decay control between the allpass filters and the last delay lines
    fX3 *= fDecaySm;
    fX4 *= fDecaySm;

    // Dealy lines (2 and 4)
    fX5 = fTankDelay2.Process(fX3);
    fX6 = fTankDelay4.Process(fX4);

    // Added Allpasses 7 - 10
    fX7 = fAllpass7.Process(fX5, fTankDiffSm, fSizeSm);
    fX8 = fAllpass8.Process(fX6, fTankDiffSm, fSizeSm);
    fX9 = fAllpass9.Process(fX7, fTankDiffSm, fSizeSm);
    fX10 = fAllpass10.Process(fX8, fTankDiffSm, fSizeSm);

    // Compute the accumulators as the outputs from the last tank nodes scaled by decay control
    fTankAcc1 = fDecaySm * fX9;
    fTankAcc2 = fDecaySm * fX10;

    // Simplified output computation compared to dattorro's
    tL = 0.5f * (fA1 + fX2 - fX3 + fX5 - fX8 + fX10);
    tR = 0.5f * (fA2 + fX1 - fX4 + fX6 - fX7 + fX9);

    // Stereo enchansment
    const float m = (tL + tR) * 0.5f;
    const float s = (tL - tR) * 1.5f;
    outL = m - s;
    outR = m + s;
}
