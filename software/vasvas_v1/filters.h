#pragma once

#include "tools.h"

class TFilterLP
{
private:
  float fSampleRate = 0; // Hz
  float fCutoff = 20000; // Hz
  float fB = 0;
  float fA = 0;
  float fOutput = 0;
public:

  inline void Reset()
  {
    fOutput = 0;
  }

  void SetParams(float samplerate, float cutoff)
  {
    fSampleRate = samplerate;
    fCutoff = cutoff;
    Update();
  }

  void Update()
  {
    // Prevent going over the Nyquist frequency
    if (fCutoff >= fSampleRate * 0.4999f)
      fCutoff = fSampleRate * 0.4999f;

    auto x = TWO_PI * fCutoff / fSampleRate;
    auto nn = (2 - cos(x));
    auto alpha = nn - sqrt(nn * nn - 1);

    fA = static_cast<float>(alpha);
    fB = 1 - fA;
  }

  inline float Process(float input)
  {
    if (Tools::IsDenormalValue(input))
      fOutput = 0;
    else
      fOutput = fB * input + fA * fOutput;
    return fOutput;
  }

};


class TFilterHP
{
private:
  float fSampleRate = 0; // Hz
  float fCutoff = 60; // Hz
  float fB = 0;
  float fA = 0;
  float fOutput = 0;
public:

  inline void Reset()
  {
    fOutput = 0;
  }

  void SetParams(float samplerate, float cutoff)
  {
    fSampleRate = samplerate;
    fCutoff = cutoff;
    Update();
  }

  void Update()
  {
    // Prevent going over the Nyquist frequency
    if (fCutoff >= fSampleRate * 0.4999f)
      fCutoff = fSampleRate * 0.4999f;

    auto x = TWO_PI * fCutoff / fSampleRate;
    auto nn = (2 - cos(x));
    auto alpha = nn - sqrt(nn * nn - 1);

    fA = static_cast<float>(alpha);
    fB = 1 - fA;
  }

  inline float Process(float input)
  {
    if (Tools::IsDenormalValue(input))
    {
      fOutput = 0;
      return fOutput;
    }
    else
    {
      fOutput = fB * input + fA * fOutput;
      return input - fOutput;
    }
  }

};