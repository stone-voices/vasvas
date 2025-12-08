#pragma once

#include "smoother.h"
#include "delayline.h"       
#include "allpass.h"
#include "filters.h"


class TReverb
{
private:
  float fSampleRate = 48000;   
  float fA1 = 0;  
  float fA2 = 0; 
  float fX1 = 0;
  float fX2 = 0;
  float fX3 = 0;
  float fX4 = 0;
  float fX5 = 0;
  float fX6 = 0;
  float fX7 = 0;
  float fX8 = 0;
  float fX9 = 0;
  float fX10 = 0;
  float fTankAcc1 = 0;
  float fTankAcc2 = 0;
  float fTankDiff = 0;
  float fTankDiffSm = 0;
  float fSize = 1;
  float fSizeSm = 1;
  float fDecayVal = 0;
  float fDecay = 0;
  float fDecaySm = 0;
  float fDelaySamples = 0;
  bool fFreeze = false;
  TFilterLP input_lowpass;
  TFilterHP input_highpass;
  TFilterLP tank_lp1;
  TFilterLP tank_lp2;
  TFilterHP tank_hp1;
  TFilterHP tank_hp2;
  TSmoother fSmootherSize;
  TSmoother fSmootherDecay;
  TSmoother fSmootherTankDiff;
  TStaticAllPass allpass1;                              
  TStaticAllPass allpass2;
  TStaticAllPass allpass3;
  TStaticAllPass allpass4;
  TResizedAllPass allpass5;
  TResizedAllPass allpass6;
  TResizedAllPass allpass7;
  TResizedAllPass allpass8;
  TResizedAllPass allpass9;
  TResizedAllPass allpass10;
  TModulatedAllPass mod_allpass1; 
  TModulatedAllPass mod_allpass2;
  TDelayLine delay1_tank;                       
  TDelayLine delay2_tank;
  TDelayLine delay3_tank;
  TDelayLine delay4_tank;
  void UpdateDecay(float value);
public:
  TReverb() {}
  ~TReverb() {}
  void SetSampleRate(float samplerate);
  void SetDecay(float value);
  void SetSize(float value);
  void SetFreeze(bool value);
  void Process(float in, float& outL, float& outR);
};



