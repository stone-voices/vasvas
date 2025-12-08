#pragma once

#include "tools.h"

class TSmoother
{
private:
  float fA = 0;
  float fB = 0;
  float fOld = 0;
public:
  void SetSmoothTime(float ms, float samplerate)
  {
    fA = std::exp(-TWO_PI / (ms * 0.001f * samplerate));
    fB = 1.0f - fA;
  }

  inline float Process(float sample)
  {
    fOld = Tools::DenormalValue(fB * sample + fA * fOld);
    return fOld;
  }
};
