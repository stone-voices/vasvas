#pragma once

#include "tools.h"
#include <cmath>


class TFastSin
{
private:
  static constexpr int TABLE_SIZE = 32768;
  static constexpr int TABLE_MASK = TABLE_SIZE - 1;
  static constexpr float TWO_PI = 6.28318530717958647692f;
  static constexpr float ANGLE_TO_INDEX = TABLE_SIZE / TWO_PI;
  static float* fSinTable;
public:
  static void Initialize()
  {
    fSinTable = Tools::CreateArray<float>(TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
      float angle = i * TWO_PI / TABLE_SIZE;
      fSinTable[i] = std::sin(angle);
    }
  }

  static float Sin(float x)
  {
    return fSinTable[static_cast<int>(x * ANGLE_TO_INDEX) & TABLE_MASK];
  }

  static float SinNorm(float x)
  {
    return fSinTable[static_cast<int>(x * TABLE_SIZE) & TABLE_MASK];
  }

};


