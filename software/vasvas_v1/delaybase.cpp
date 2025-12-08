#include "delaybase.h"

#ifndef __arm__
#include <stdexcept>
#define ALLOC_ERR "Failed allocated memory for delay line"
#endif

TDelayCell::~TDelayCell()
{
  SetSize(0);
}

void TDelayCell::Reset()
{
  if (fBuff)
    Tools::ZeroArray(fBuff, fSize);
}

void TDelayCell::SetSize(int size)
{
  if (fBuff)
    Tools::DeleteArray(fBuff); 
  fBuff = nullptr;
  fSize = 0;
  fIterator = 0;
  if (size <= 0)
    return;
  const int len = static_cast<int>(log2(size)) + 1;
  fSize = 1 << len;
  fBuff = Tools::CreateArray<float>(fSize);
#ifndef __arm__
  if (!fBuff)
    throw std::runtime_error{ ALLOC_ERR };
#endif
  fMask = fSize - 1;
  Reset();
}
