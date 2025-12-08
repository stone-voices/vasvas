#pragma once

#define _USE_MATH_DEFINES
#include <cstdlib>
#include <cstring> 
#include <limits>
#include <cmath>

constexpr float TWO_PI = 6.28318530717959f;

extern void* alloc_once(size_t size);

namespace Tools
{
  template<typename T>
  T* CreateArray(size_t count)
  {
    void* memory = alloc_once(count * sizeof(T));
    if (!memory)
      return nullptr;

    std::memset(memory, 0, count * sizeof(T));
    return static_cast<T*>(memory);
  }

  template<typename T>
  inline void DeleteArray(T* &ptr)
  {
#ifndef __arm__
    if (ptr)
      free(ptr);
    ptr = nullptr;
#endif
  }

  template<typename T>
  inline void ZeroArray(T* a, int count)
  {
    memset(a, 0, count * sizeof(T));
  }

  template<typename T>
  inline void Gain(T* buffer, T fGain, int size)
  {
    for (int i = 0; i < size; ++i)
    {
      buffer[i] *= fGain;
    }
  }

  template<typename T>
  inline void Mix(T* target, T* source, T fGain, int size)
  {
    for (int i = 0; i < size; ++i)
      target[i] += source[i] * fGain;
  }

  template<typename T>
  inline void Copy(T* dest, T* source, int size)
  {
    memcpy(dest, source, size * sizeof(T));
  }

  template<typename T>
  inline void DenormalArray(T* a, int size)
  {
    const T threshold = std::numeric_limits<T>::min() * T(10);
    for (int i = 0; i < size; ++i)
      if (std::abs(a[i]) < threshold) 
        a[i] = T(0);
  }

  template<typename T>
  inline T DenormalValue(T value)
  {
    const T threshold = std::numeric_limits<T>::min() * T(10);
    if (std::abs(value) < threshold)
      return T(0);
    else
      return value;
  }

  template<typename T>
  inline bool IsDenormalValue(T value)
  {
    const T threshold = std::numeric_limits<T>::min() * T(10);
    return (std::abs(value) < threshold);
  }

  inline float DB2Gainf(float input)
  {
    return powf(10, input * 0.05f);
  }

}

