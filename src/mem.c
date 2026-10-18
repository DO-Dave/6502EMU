#include "../incl/mem.h"

void initialize_memory(MEM *mem)
{
  memset(mem->Data, 0, MAX_MEM * sizeof(mem->Data[0]));
}

void print_memory(const MEM *mem, u32 start, u32 end)
{
  if (mem == NULL || start > end || end >= MAX_MEM)
  {
    printf("Invalid memory range.\n");
    return;
  }

  printf("\n");
  printf("       ");
  for (int i = 0; i < 16; i++)
    printf("%02X ", i);

  printf(" | ASCII\n");
  printf("-------");
  for (int i = 0; i < 16; i++)
    printf("---");

  printf(" | ----------------\n");

  uint32_t row_start = start & ~0xFU;
  uint32_t row_end = end & ~0xFU;

  for (uint32_t row = row_start; row <= row_end; row += 16)
  {
    printf("%04X:  ", (unsigned int)row);

    for (int col = 0; col < 16; col++)
    {
      uint32_t addr = row + col;

      if (addr >= start && addr <= end)
        printf("%02X ", mem->Data[addr]);
      else
        printf("   ");
    }

    printf(" | ");

    for (int col = 0; col < 16; col++)
    {
      uint32_t addr = row + col;

      if (addr >= start && addr <= end)
      {
        unsigned char c = mem->Data[addr];
        printf("%c", isprint(c) ? c : '.');
      }
      else
      {
        printf(" ");
      }
    }

    printf("\n");
  }

  printf("\n");
}