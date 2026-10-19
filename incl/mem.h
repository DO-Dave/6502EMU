#ifndef MEM_H
#define MEM_H
#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
typedef uint8_t Byte;
typedef uint16_t Word;
typedef uint32_t u32;
typedef signed int s32;
typedef uint16_t u16;
typedef uint8_t u8;

#define MAX_MEM (1024u * 64u) // kb

typedef struct
{
  Byte Data[MAX_MEM];
} MEM;
void initialize_memory(MEM *mem);
void print_memory(const char *filename, const MEM *mem, u32 start, u32 end);
#endif // MEM_H