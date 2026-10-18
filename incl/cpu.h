#ifndef CPU_H
#define CPU_H
#include "mem.h"

#define COLOR_RESET "\x1b[0m"
#define COLOR_BOLD "\x1b[1m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_BLUE "\x1b[34m"

#define NULL_INS 0x00
#define INS_LDA_IM 0xA9
#define INS_LDA_ZP 0xA5
#define INS_LDA_ZPX 0xB5
#define INS_LDA_ABS 0xAD
#define INS_LDA_ABS_X 0xBD
#define INS_LDA_ABS_Y 0xB9
#define INS_LDA_IND_X 0xA1
#define INS_LDA_IND_Y 0xB1

#define INS_JSR_A 0x20
// cycle count
#define INS_LDA_IM_CYC 2
#define INS_LDA_ZP_CYC 3
#define INS_LDA_ZPX_CYC 4
#define INS_LDA_ABS_CYC 4
#define INS_LDA_ABS_X_CYC 4 // or 4+1 if page bound cross
#define INS_LDA_ABS_Y_CYC 4 // or 4+1 if page bound cross
#define INS_LDA_IND_X_CYC 6
#define INS_LDA_IND_Y_CYC 4 // or 4+1 if page bound cross

#define INS_JSR_A_CYC 6




typedef struct
{

  Word PC; // program counter
  Word SP; // stack pointer

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
// simply reads a byte at a given address but does not increment the PC
Byte read_byte(MEM *mem, const u16 addr, s32 *cycles);
void reset_cpu(CPU *cpu, MEM *mem);
void print_cpu(const CPU *cpu);

void lda_set_flags(CPU *cpu);
#endif // CPU_H