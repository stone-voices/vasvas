#pragma once

#include "delaybase.h"

class TDelayLine: public TDelayCell
{
private:                   
  float fDelay = 0;
public:              
  void SetParams(float samplerate, float delay);
  float Process(float sample);
};
 
