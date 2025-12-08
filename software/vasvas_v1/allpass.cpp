#include "allpass.h"
#include "fastsin.h"
#include "tools.h"

// TStaticAllPass

void TStaticAllPass::SetParams(float samplerate, float delay, float feedback)
{
  fFeedback = feedback;
  fDelay = int(round(samplerate / FsD * delay));
  SetSize(fDelay);
}

float TStaticAllPass::Process(float sample)
{
  int i = fIterator - fDelay;
  if (i < 0)
    i += fSize;

  float out = fBuff[i];          
  float in = sample - out * fFeedback;  
  float result = in * fFeedback + out; 
  fBuff[fIterator] = in;                    

  ++fIterator &= fMask;

  return Tools::DenormalValue(result);
}


// TResizedAllPass

void TResizedAllPass::SetParams(float samplerate, float delay)
{
  fDelay = samplerate / FsD * delay;
  SetSize(int(round(MaxSize * fDelay)));
}

float TResizedAllPass::Process(float sample, float feedback, float sizecoeff)
{
  float x = fIterator - fDelay * sizecoeff;

  if (x < 0.f)
    x += fSize;

  int i = static_cast<int>(x);
  const float frac = x - i;
  --i;
  const float y0 = fBuff[i++ & fMask];
  const float y1 = fBuff[i++ & fMask];
  const float y2 = fBuff[i++ & fMask];
  const float y3 = fBuff[i & fMask];
  float out = Hermite(frac, y0, y1, y2, y3);

  fBuff[fIterator] = sample;
  float in = sample - out * feedback;
  float result = in * feedback + out;
  fBuff[fIterator] = in;

  ++fIterator &= fMask;

  return Tools::DenormalValue(result);
}


// TModulatedAllPass

void TModulatedAllPass::SetParams(float samplerate, float delay, float rate, float depth)
{
  fDepth = depth;
  fLfoInc = TWO_PI * rate / samplerate;
  fDelay = samplerate / FsD * delay;
  SetSize(int(round(MaxSize * fDelay + fDepth)));
}

float TModulatedAllPass::Process(float sample, float sizecoeff)
{
  float x = fIterator - fDelay * sizecoeff;

  x += fDepth * TFastSin::Sin(fLfoAngle);
  fLfoAngle += fLfoInc;
  if (fLfoAngle > TWO_PI)
    fLfoAngle -= TWO_PI;

  if (x < 0.f)
    x += fSize;

  int i = static_cast<int>(x);
  const float frac = x - i;
  --i;
  const float y0 = fBuff[i++ & fMask];
  const float y1 = fBuff[i++ & fMask];
  const float y2 = fBuff[i++ & fMask];
  const float y3 = fBuff[i & fMask];
  float out = Hermite(frac, y0, y1, y2, y3);

  fBuff[fIterator] = sample;
  float in = sample - out * fFeedback;
  float result = in * fFeedback + out;
  fBuff[fIterator] = in;

  ++fIterator &= fMask;

  return Tools::DenormalValue(result);
}

