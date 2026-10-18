#ifndef CPU_H
#define CPU_H
#include "inst.h"
#include "mem.h"

#define COLOR_RESET "\x1b[0m"
#define COLOR_BOLD "\x1b[1m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_BLUE "\x1b[34m"
typedef struct
{

  Word PC;
  Word SP;

  Byte A, X, Y; // registers: accumulator, index X, index Y
  // Status flags
  // The : 1 just means that regardles of the size of byte
  // this variables just occupies 1 bit
  Byte C : 1; // carry
  Byte Z : 1; // zero
  Byte I : 1; // interrupt disable
  Byte D : 1; // decimal mode
  Byte B : 1; // Break command
  Byte V : 1; // overflow
  Byte N : 1; // negative
} CPU;

s32 cpu_execute(CPU *cpu, MEM *mem, s32 cycles);
Byte fetch_byte(CPU *cpu, MEM *mem, s32 *cycles);
Word fetch_word(CPU *cpu, MEM *mem, s32 *cycles);

Byte read_byte(MEM *mem, const u16 addr, s32 *cycles);
void reset_cpu(CPU *cpu, MEM *mem);
void print_cpu(const CPU *cpu);

void lda_set_flags(CPU *cpu);
void ldx_set_flags(CPU *cpu);
void ldy_set_flags(CPU *cpu);

#endif // CPU_H