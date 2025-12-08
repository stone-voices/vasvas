#include "delayline.h"


void TDelayLine::SetParams(float samplerate, float delay)
{
  fDelay = samplerate / FsD * delay;
  SetSize(MaxSize * int(round(fDelay)));
}

float TDelayLine::Process(float sample)
{
  float x = fIterator - fDelay;

  if (x < 0.f)
    x += fSize;

  int i = static_cast<int>(x);
  const float frac = x - i;
  --i;
  const float y0 = fBuff[i++ & fMask];
  const float y1 = fBuff[i++ & fMask];
  const float y2 = fBuff[i++ & fMask];
  const float y3 = fBuff[i & fMask];
  float result = Hermite(frac, y0, y1, y2, y3);

  fBuff[fIterator] = sample;

  ++fIterator &= fMask;

  if (fDelay == 0)
    result = sample;

  return result;
}



