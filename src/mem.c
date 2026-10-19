#include "../incl/mem.h"

void initialize_memory(MEM *mem)
{
  memset(mem->Data, 0, MAX_MEM * sizeof(mem->Data[0]));
}

void print_memory(const char *filename, const MEM *mem, u32 start, u32 end)
{
  if (filename == NULL || mem == NULL || start > end || end >= MAX_MEM)
  {
    printf("Invalid memory range or filename.\n");
    return;
  }

  FILE *file = fopen(filename, "w");

  if (file == NULL)
  {
    perror("Failed to open file");
    return;
  }

  fprintf(file, "\n");
  fprintf(file, "       ");

  for (int i = 0; i < 16; i++)
    fprintf(file, "%02X ", i);

  fprintf(file, " | ASCII\n");
  fprintf(file, "-------");

  for (int i = 0; i < 16; i++)
    fprintf(file, "---");

  fprintf(file, " | ----------------\n");

  uint32_t row_start = start & ~0xFU;
  uint32_t row_end = end & ~0xFU;

  for (uint32_t row = row_start; row <= row_end; row += 16)
  {
    fprintf(file, "%04X:  ", (unsigned int)row);

    for (int col = 0; col < 16; col++)
    {
      uint32_t addr = row + col;

      if (addr >= start && addr <= end)
        fprintf(file, "%02X ", mem->Data[addr]);
      else
        fprintf(file, "   ");
    }

    fprintf(file, " | ");

    for (int col = 0; col < 16; col++)
    {
      uint32_t addr = row + col;

      if (addr >= start && addr <= end)
      {
        unsigned char c = mem->Data[addr];
        fprintf(file, "%c", isprint(c) ? c : '.');
      }
      else
      {
        fprintf(file, " ");
      }
    }

    fprintf(file, "\n");
  }

  fprintf(file, "\n");

  fclose(file);
}