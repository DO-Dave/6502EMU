#include "../incl/cpu.h"

const char *instruction_name(Byte instr)
{
  switch (instr)
  {
    case INS_LDA_IM:
      return "LDA_IM";
    case INS_LDA_ZP:
      return "LDA_ZP";
    case INS_LDA_ZPX:
      return "LDA_ZPX";
    case INS_JSR_A:
      return "JSR_A";
    case INS_LDA_ABS:
      return "LDA_ABS";
    case INS_LDA_ABS_X:
      return "LDA_ABS_X";
    case INS_LDA_ABS_Y:
      return "LDA_ABS_Y";
    case INS_LDA_IND_X:
      return "LDA_IND_X";
    case INS_LDA_IND_Y:
      return "LDA_IND_Y";
    default:
      return "UNKNOWN";
  }
}

Byte fetch_byte(CPU *cpu, MEM *mem, s32 *cycles)
{
  assert(cpu->PC >= 0 && cpu->PC <= MAX_MEM);
  Byte data = mem->Data[cpu->PC];
  cpu->PC++;
  (*cycles)--;
  return data;
}

Word fetch_word(CPU *cpu, MEM *mem, s32 *cycles)
{
  // 6502 is little endian
  // My system : lscpu | grep Endian : Little endian
  // first the lower 8 bits
  Word data = mem->Data[cpu->PC];
  cpu->PC++;
  // the the high 8 bits
  data |= (mem->Data[cpu->PC] << 8);
  (*cycles) -= 2; // needs two cycles for the two memory reads
  return data;
}

Byte read_byte(MEM *mem, const u16 addr, s32 *cycles)
{
  Byte data = mem->Data[addr];
  (*cycles)--;
  return data;
}

void write_word(MEM *mem, Word value, const u32 address, s32 *cycles)
{
  // again be aware of little endian
  mem->Data[address] = value & 0xFF;
  mem->Data[address + 1] = (value >> 8);
  (*cycles) -= 2;
}

void lda_set_flags(CPU *cpu)
{
  cpu->Z = (cpu->A == 0);        // Set if A is zero
  cpu->N = (cpu->A & 0x80) != 0; // Set if the most significant bit is set
}

s32 cpu_execute(CPU *cpu, MEM *mem, s32 cycles)
{
  Byte instr;
  const s32 requested_cycles = cycles;
  while (cycles > 0)
  {
    instr = fetch_byte(cpu, mem, &cycles);
    const char *inst_name = instruction_name(instr);
    if (strcmp(inst_name, "UNKNOWN") == 0)
    {
      printf(COLOR_MAGENTA COLOR_BOLD "UNKNOWN INSTRUCTION: 0x%02X" COLOR_RESET "\n", instr);
    }
    else
    {
      printf("EXE: " COLOR_CYAN "%-8s" COLOR_RESET " | Opcode: " COLOR_YELLOW "0x%02X" COLOR_RESET "\n", inst_name, instr);
    }

    switch (instr)
    {
      case INS_LDA_IM:
      {
        Byte value = fetch_byte(cpu, mem, &cycles);
        cpu->A = value;
        lda_set_flags(cpu);
        break;
      }
      case INS_LDA_ZP:
      {
        Byte zero_page_addr = fetch_byte(cpu, mem, &cycles);
        cpu->A = read_byte(mem, zero_page_addr, &cycles);
        lda_set_flags(cpu);
        break;
      }
      case INS_LDA_ZPX:
      {
        Byte zero_page_addr = fetch_byte(cpu, mem, &cycles);
        // wrapping works since byte is uint8_t
        zero_page_addr += cpu->X;
        cpu->A = read_byte(mem, zero_page_addr, &cycles);
        cycles--;
        lda_set_flags(cpu);
        break;
      }
      case INS_JSR_A:
      {
        Word subroutine_addr = fetch_word(cpu, mem, &cycles);
        write_word(mem, cpu->PC - 1, cpu->SP, &cycles);
        cpu->PC = subroutine_addr;
        cycles--;
        break;
      }
      case INS_LDA_ABS:
      {
        Word abs_addr = fetch_word(cpu, mem, &cycles);
        cpu->A = read_byte(mem, abs_addr, &cycles);
        break;
      }
      case INS_LDA_ABS_X:
      {
        Word abs_addr = fetch_word(cpu, mem, &cycles);
        u16 result = abs_addr + cpu->X;
        if ((abs_addr >> 8) != (result >> 8))
        {
          // We need 1 more cycle step if abs_addr + cpu.X exceeds a u8:
          // abs_addr = 0x1101, cpu.X = 0xFF += 0x1200 crosses a page
          cycles--;
        }
        cpu->A = read_byte(mem, result, &cycles);
        break;
      }
      case INS_LDA_ABS_Y:
      {
        Word abs_addr = fetch_word(cpu, mem, &cycles);
        u16 result = abs_addr + cpu->Y;
        if ((abs_addr >> 8) != (result >> 8))
        {
          // We need 1 more cycle step if abs_addr + cpu.X exceeds a u8:
          // abs_addr = 0x1101, cpu.X = 0xFF += 0x1200 crosses a page
          cycles--;
        }
        cpu->A = read_byte(mem, result, &cycles);
        break;
      }
      case INS_LDA_IND_X:
      {
        break;
      }
      case INS_LDA_IND_Y:
      {
        break;
      }
      default:
        return requested_cycles - cycles;
    }
  }
  return requested_cycles - cycles;
}

void reset_cpu(CPU *cpu, MEM *mem)
{

  cpu->PC = 0xFFFC;
  cpu->SP = 0x0100;

  cpu->A = cpu->X = cpu->Y = 0;

  cpu->C = cpu->Z = cpu->I = cpu->B = cpu->V = cpu->D = cpu->N = 0;
  initialize_memory(mem);
}

void print_cpu(const CPU *cpu)
{
  if (cpu == NULL)
  {
    printf(COLOR_MAGENTA "CPU: NULL\n" COLOR_RESET);
    return;
  }

  printf("\n" COLOR_BOLD "========== CPU STATE ==========\n" COLOR_RESET);

  printf(COLOR_CYAN "PC: " COLOR_RESET COLOR_YELLOW "0x%04X" COLOR_RESET "    " COLOR_CYAN "SP: " COLOR_RESET COLOR_YELLOW
                    "0x%02X" COLOR_RESET "\n",
         cpu->PC, cpu->SP);

  printf("\n" COLOR_BOLD "Registers:\n" COLOR_RESET);

  printf(COLOR_MAGENTA "A: " COLOR_RESET COLOR_YELLOW "0x%02X (%3u)    " COLOR_RESET COLOR_MAGENTA "X: " COLOR_RESET COLOR_YELLOW
                       "0x%02X (%3u)    " COLOR_RESET COLOR_MAGENTA "Y: " COLOR_RESET COLOR_YELLOW "0x%02X (%3u)\n" COLOR_RESET,
         cpu->A, cpu->A, cpu->X, cpu->X, cpu->Y, cpu->Y);

  printf("\n" COLOR_BOLD "Processor Status Flags:\n" COLOR_RESET);

  printf(COLOR_CYAN "N: " COLOR_YELLOW "%d" COLOR_RESET "  " COLOR_CYAN "V: " COLOR_YELLOW "%d" COLOR_RESET "  " COLOR_CYAN
                    "B: " COLOR_YELLOW "%d" COLOR_RESET "  " COLOR_CYAN "D: " COLOR_YELLOW "%d" COLOR_RESET "  " COLOR_CYAN
                    "I: " COLOR_YELLOW "%d" COLOR_RESET "  " COLOR_CYAN "Z: " COLOR_YELLOW "%d" COLOR_RESET "  " COLOR_CYAN
                    "C: " COLOR_YELLOW "%d" COLOR_RESET "\n\n",
         cpu->N, cpu->V, cpu->B, cpu->D, cpu->I, cpu->Z, cpu->C);
}
