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
  TFilterLP fInpLPF;
  TFilterHP fInpHPF;
  TFilterLP fTankLPF1;
  TFilterLP fTankLPF2;
  TFilterHP fTankHPF1;
  TFilterHP fTankHPF2;
  TSmoother fSmootherSize;
  TSmoother fSmootherDecay;
  TSmoother fSmootherTankDiff;
  TStaticAllPass fAllpass1;                              
  TStaticAllPass fAllpass2;
  TStaticAllPass fAllpass3;
  TStaticAllPass fAllpass4;
  TResizedAllPass fAllpass5;
  TResizedAllPass fAllpass6;
  TResizedAllPass fAllpass7;
  TResizedAllPass fAllpass8;
  TResizedAllPass fAllpass9;
  TResizedAllPass fAllpass10;
  TModulatedAllPass fModAllpass1; 
  TModulatedAllPass fModAllpass2;
  TDelayLine fTankDelay1;                       
  TDelayLine fTankDelay2;
  TDelayLine fTankDelay3;
  TDelayLine fTankDelay4;
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
