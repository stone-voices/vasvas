#include "tools.h"


#ifdef __arm__

// Daisy Seed (ARM)
#include "daisy_seed.h"

DSY_SDRAM_BSS uint8_t memory_pool[48 * 1024 * 1024]; // 48MB SDRAM
size_t memory_used = 0;

void* alloc_once(size_t size)
{
  if (memory_used + size > sizeof(memory_pool))
    return nullptr;
  void* ptr = &memory_pool[memory_used];
  memory_used += size;
  return ptr;
}

#else

// Компиляция для компьютера
void* alloc_once(size_t size)
{
  return malloc(size);
}

#endif