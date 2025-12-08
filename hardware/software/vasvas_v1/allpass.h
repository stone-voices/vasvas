#pragma once

#include "delaybase.h"

class TStaticAllPass : public TDelayCell
{
protected:
  float fFeedback = 0;
  int fDelay = 0;
public:
  void SetParams(float samplerate, float delay, float feedback);
  float Process(float sample);
};

class TResizedAllPass : public TDelayCell
{
private:
  float fDelay = 0;
public:
  void SetParams(float samplerate, float delay);
  float Process(float sample, float feedback, float sizecoeff);
};

class TModulatedAllPass : public TDelayCell
{
private:
  float fLfoInc = 0;
  float fLfoAngle = 0;
  float fDepth = 0;
  float fDelay = 0;
  const float fFeedback = 0.7f;
public:
  void SetParams(float samplerate, float delay, float rate, float depth);
  float Process(float sample, float sizecoeff);
};


