#pragma once

#include "tools.h"

class TDelayCell
{
protected:
  
  int fSize = 0;
  int fMask = 0;
  int fIterator = 0;
  float* fBuff = nullptr;
  static inline float Hermite(const float x, const float y0, const float y1, const float y2, const float y3) noexcept
  {
    // 4-point, 3rd-order Hermite (x-form)
    return (((1.5f * (y1 - y2) + 0.5f * (y3 - y0)) * x + y0 - 2.5f * y1 + 2.f * y2 - 0.5f * y3) * x + 0.5f * (y2 - y0)) * x + y1;
  }
public:
  static constexpr float FsD = 29761; // Dattorro's original sampling frequency
  static constexpr int MaxSize = 4;
  TDelayCell() {}
  ~TDelayCell();
  void Reset();
  void SetSize(int size);
};
