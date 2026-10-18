#include "../incl/test.h"
bool check_lda_flags(const CPU *cpu, const CPU *cpu_copy)
{
  // Flags that can change
  const bool flags_set = (cpu->Z == (!cpu->A)) && (cpu->N == ((cpu->A & 0x80) != 0));

  const bool flags_unchanged =
      cpu->C == cpu_copy->C && cpu->I == cpu_copy->I && cpu->D == cpu_copy->D && cpu->B == cpu_copy->B && cpu->V == cpu_copy->V;

  return flags_set && flags_unchanged;
}

bool check_all_flags_same(const CPU *cpu, const CPU *cpu_copy)
{
  return (cpu->C == cpu_copy->C) && (cpu->Z == cpu_copy->Z) && (cpu->I == cpu_copy->I) && (cpu->D == cpu_copy->D) &&
         (cpu->B == cpu_copy->B) && (cpu->V == cpu_copy->V) && (cpu->N == cpu_copy->N);
}

void test_do_nothing(CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  cpu->PC = 0xFFFC;
  CPU cpu_copy = *cpu;
  s32 used_cycles = cpu_execute(cpu, mem, 0);
  assert(used_cycles == 0);
  assert(check_all_flags_same(cpu, &cpu_copy));
  print_test_pass("DO_NOTHING", used_cycles);
}

void test_exe_full_inst_always_lda_im(CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u16 pc_start = 0XFFFC;
  u8 expected = 0x67;
  // SetUp
  cpu->PC = pc_start;
  mem->Data[pc_start] = INS_LDA_IM;
  mem->Data[0xFFFD] = expected;

  // Run
  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_IM_CYC - 1);

  // Check
  assert(used_cycles == INS_LDA_IM_CYC);
  assert(cpu->A == expected);
  assert(check_lda_flags(cpu, &cpu_copy));

  print_test_pass("EXE_FULL_LDA_IM:", used_cycles);
}

void test_exe_full_inst_always_lda_zp(CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u32 expected = 0x2;
  u16 zp_offset = 0x0042;
  u16 pc_start = 0xFFFC;

  cpu->PC = pc_start;

  // SetUp
  mem->Data[pc_start] = INS_LDA_ZP;
  mem->Data[zp_offset] = expected;
  mem->Data[0xFFFD] = zp_offset;

  // Run
  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_ZP_CYC - 1);

  // Check
  assert(used_cycles == INS_LDA_ZP_CYC);
  assert(cpu->A == expected);
  assert(check_lda_flags(cpu, &cpu_copy));

  print_test_pass("EXE_FULL_LDA_ZP:", used_cycles);
}

void test_exe_full_inst_always_lda_zpx(CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u32 expected = 0x3;
  u16 zp_offset = 0x0042;
  u16 x_reg_value = 0x5;

  // Setting up the X register manually
  cpu->X = x_reg_value;

  u16 pc_start = 0xFFFC;
  cpu->PC = pc_start;

  // SetUp
  mem->Data[pc_start] = INS_LDA_ZPX;
  mem->Data[0xFFFD] = zp_offset;
  mem->Data[zp_offset + x_reg_value] = expected;

  // Run
  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_ZPX_CYC - 3);

  // Check
  assert(used_cycles == INS_LDA_ZPX_CYC);
  assert(cpu->A == expected);
  assert(check_lda_flags(cpu, &cpu_copy));

  print_test_pass("EXE_FULL_LDA_ZPX:", used_cycles);
}

void test_exe_full_inst_always_jsr_a(CPU *cpu, MEM *mem)
{
  /*
   * Important this garantuees that if i start a inst always finishes the no matter if we overshoot the cycles that the cpu was
   * given
   */
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u32 expected = 0x4;

  u16 jmp_addr = 0x4342;
  u16 pc_start = 0xFFFC;
  cpu->PC = pc_start;

  mem->Data[pc_start] = INS_JSR_A;

  // First low bits, then high bits
  mem->Data[0xFFFD] = jmp_addr & 0xFF;
  mem->Data[0xFFFE] = (jmp_addr >> 8) & 0xFF;

  mem->Data[jmp_addr] = INS_LDA_IM;
  mem->Data[jmp_addr + 1] = expected;

  s32 used_cycles = cpu_execute(cpu, mem, INS_JSR_A_CYC + 1);

  // Check
  assert(used_cycles == (INS_JSR_A_CYC + INS_LDA_IM_CYC));
  assert(cpu->A == expected);
  assert(check_all_flags_same(cpu, &cpu_copy));

  print_test_pass("EXE_FULL_JSR_A:", used_cycles);
}

void test_garbage_inst(CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u16 pc_start = 0XFFFC;
  mem->Data[pc_start] = NULL_INS;
  s32 used_cycles = cpu_execute(cpu, mem, 5);
  assert(used_cycles == 1);
  assert(check_all_flags_same(cpu, &cpu_copy));
  print_test_pass("GARBAGE_INS:", used_cycles);
}

void test_lda_im(CPU *cpu, MEM *mem, u8 expected)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u16 pc_start = 0XFFFC;

  // SetUp
  cpu->PC = pc_start;
  mem->Data[pc_start] = INS_LDA_IM;
  mem->Data[0xFFFD] = expected;

  // Run
  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_IM_CYC);

  // Check
  assert(used_cycles == INS_LDA_IM_CYC);
  assert(cpu->A == expected);
  assert(check_lda_flags(cpu, &cpu_copy));

  print_test_pass("LDA_IM:", used_cycles);
}

void test_lda_zp(CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u32 expected = 0x2;
  u16 zp_offset = 0x0042;
  u16 pc_start = 0xFFFC;

  cpu->PC = pc_start;

  // SetUp
  mem->Data[pc_start] = INS_LDA_ZP;
  mem->Data[zp_offset] = expected;
  mem->Data[0xFFFD] = zp_offset;

  // Run
  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_ZP_CYC);

  // Check
  assert(used_cycles == INS_LDA_ZP_CYC);
  assert(cpu->A == expected);
  assert(check_lda_flags(cpu, &cpu_copy));

  print_test_pass("LDA_ZP:", used_cycles);
}

void test_lda_zpx(CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u32 expected = 0x3;
  u16 zp_offset = 0x0042;
  u16 x_reg_value = 0x5;

  // Setting up the X register manually
  cpu->X = x_reg_value;

  u16 pc_start = 0xFFFC;
  cpu->PC = pc_start;

  // SetUp
  mem->Data[pc_start] = INS_LDA_ZPX;
  mem->Data[0xFFFD] = zp_offset;
  mem->Data[zp_offset + x_reg_value] = expected;

  // Run
  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_ZPX_CYC);

  // Check
  assert(used_cycles == INS_LDA_ZPX_CYC);
  assert(cpu->A == expected);
  assert(check_lda_flags(cpu, &cpu_copy));

  print_test_pass("LDA_ZPX:", used_cycles);
}

void test_lda_zpx_wraping(CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;

  u32 expected = 0x3;
  u8 zp_offset = 0x80;
  u8 x_reg_value = 0xFF;
  u8 effective_addr = zp_offset + x_reg_value;
  u16 pc_start = 0xFFFC;

  // Setting up the X register manually
  cpu->X = x_reg_value;
  cpu->PC = pc_start;

  // SetUp
  mem->Data[pc_start] = INS_LDA_ZPX;
  mem->Data[0xFFFD] = zp_offset;
  mem->Data[effective_addr] = expected;

  // Run
  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_ZPX_CYC);

  // Check
  assert(used_cycles == INS_LDA_ZPX_CYC);
  assert(cpu->A == expected);
  assert(check_lda_flags(cpu, &cpu_copy));

  print_test_pass("LDA_ZPX_Overflow:", used_cycles);
}

void test_lda_abs(CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u8 expected = 0x67;
  u16 pc_start = 0XFFFC;
  u16 addr = 0x4480;
  // SetUp
  cpu->PC = pc_start;
  mem->Data[pc_start] = INS_LDA_ABS;
  // First low bits, then high bits
  mem->Data[0xFFFD] = addr & 0xFF;
  mem->Data[0xFFFE] = (addr >> 8) & 0xFF;
  mem->Data[addr] = expected;
  // Run
  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_ABS_CYC);

  // Check
  assert(used_cycles == INS_LDA_ABS_CYC);
  assert(cpu->A == expected);
  assert(check_lda_flags(cpu, &cpu_copy));
  print_test_pass("LDA_ABS:", used_cycles);
}

void test_lda_abs_x(CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u8 expected = 0x67;
  u8 add_cycle = 0;
  u16 pc_start = 0XFFFC;
  u16 addr = 0x4480;
  u8 x_reg_offset = 0x01;
  u16 resulting_addr = addr + x_reg_offset;
  if ((addr >> 8) != (resulting_addr >> 8)) add_cycle++;
  // SetUp
  cpu->PC = pc_start;
  cpu->X = x_reg_offset;
  mem->Data[pc_start] = INS_LDA_ABS_X;
  // First low bits, then high bits
  mem->Data[0xFFFD] = addr & 0xFF;
  mem->Data[0xFFFE] = (addr >> 8) & 0xFF;
  mem->Data[resulting_addr] = expected;
  // Run
  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_ABS_X_CYC);

  // Check
  assert(used_cycles == INS_LDA_ABS_X_CYC + add_cycle);
  assert(cpu->A == expected);
  assert(check_lda_flags(cpu, &cpu_copy));
  print_test_pass("LDA_ABS_X:", used_cycles);
}

void test_lda_abs_x_5_cyc(CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u8 expected = 0x69;
  u8 add_cycle = 0;
  u16 pc_start = 0XFFFC;
  u16 addr = 0x4402;
  u8 x_reg_offset = 0xFF; // for garantueeing the crossing the page
  u16 resulting_addr = addr + x_reg_offset;
  if ((addr >> 8) != (resulting_addr >> 8)) add_cycle++;
  // SetUp
  cpu->PC = pc_start;
  cpu->X = x_reg_offset;
  mem->Data[pc_start] = INS_LDA_ABS_X;
  // First low bits, then high bits
  mem->Data[0xFFFD] = addr & 0xFF;
  mem->Data[0xFFFE] = (addr >> 8) & 0xFF;
  mem->Data[resulting_addr] = expected;
  // Run
  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_ABS_X_CYC);

  // Check
  assert(used_cycles == INS_LDA_ABS_X_CYC + add_cycle);
  assert(cpu->A == expected);
  assert(check_lda_flags(cpu, &cpu_copy));
  print_test_pass("LDA_ABS_X_5_CYC:", used_cycles);
}

void test_lda_abs_y(CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u8 expected = 0x67;
  u8 add_cycle = 0;
  u16 pc_start = 0XFFFC;
  u16 addr = 0x4480;
  u8 y_reg_offset = 0x01;
  u16 resulting_addr = addr + y_reg_offset;
  if ((addr >> 8) != (resulting_addr >> 8)) add_cycle++;
  // SetUp
  cpu->PC = pc_start;
  cpu->Y = y_reg_offset;
  mem->Data[pc_start] = INS_LDA_ABS_Y;
  // First low bits, then high bits
  mem->Data[0xFFFD] = addr & 0xFF;
  mem->Data[0xFFFE] = (addr >> 8) & 0xFF;
  mem->Data[resulting_addr] = expected;
  // Run
  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_ABS_Y_CYC);

  // Check
  assert(used_cycles == INS_LDA_ABS_Y_CYC + add_cycle);
  assert(cpu->A == expected);
  assert(check_lda_flags(cpu, &cpu_copy));
  print_test_pass("LDA_ABS_X:", used_cycles);
}

void test_lda_abs_y_5_cyc(CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u8 expected = 0x69;
  u8 add_cycle = 0;
  u16 pc_start = 0XFFFC;
  u16 addr = 0x4402;
  u8 y_reg_offset = 0xFF; // for garantueeing the crossing the page
  u16 resulting_addr = addr + y_reg_offset;
  if ((addr >> 8) != (resulting_addr >> 8)) add_cycle++;
  // SetUp
  cpu->PC = pc_start;
  cpu->Y = y_reg_offset;
  mem->Data[pc_start] = INS_LDA_ABS_Y;
  // First low bits, then high bits
  mem->Data[0xFFFD] = addr & 0xFF;
  mem->Data[0xFFFE] = (addr >> 8) & 0xFF;
  mem->Data[resulting_addr] = expected;
  // Run
  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_ABS_Y_CYC);

  // Check
  assert(used_cycles == INS_LDA_ABS_Y_CYC + add_cycle);
  assert(cpu->A == expected);
  assert(check_lda_flags(cpu, &cpu_copy));
  print_test_pass("LDA_ABS_X_5_CYC:", used_cycles);
}

void test_jsr_a(CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u32 expected = 0x4;

  u16 jmp_addr = 0x4342;
  u16 pc_start = 0xFFFC;
  cpu->PC = pc_start;

  mem->Data[pc_start] = INS_JSR_A;

  // First low bits, then high bits
  mem->Data[0xFFFD] = jmp_addr & 0xFF;
  mem->Data[0xFFFE] = (jmp_addr >> 8) & 0xFF;

  mem->Data[jmp_addr] = INS_LDA_IM;
  mem->Data[jmp_addr + 1] = expected;

  s32 used_cycles = cpu_execute(cpu, mem, INS_JSR_A_CYC + INS_LDA_IM_CYC);

  // Check
  assert(used_cycles == INS_JSR_A_CYC + INS_LDA_IM_CYC);
  assert(cpu->A == expected);
  assert(check_all_flags_same(cpu, &cpu_copy));

  print_test_pass("JSR_A:", used_cycles);
}

void test_lda_ind_x(CPU *cpu, MEM *mem)
{
  /*
   * The 16 bits + X register after the instruction are the next address; The go to that address and look at the 8 bits after it
   * that is the value we want in A
   */
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  cpu->X = 0x04;
  u16 pc_start = 0XFFFC;
  u8 expected = 0x67;
  mem->Data[pc_start] = INS_LDA_IND_X;
  mem->Data[0xFFFD] = 0x02;
  mem->Data[0x0006] = 0x00;
  mem->Data[0x0007] = 0x80;
  mem->Data[0x8000] = expected;
  // Run
  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_IND_X_CYC);

  // Check
  assert(used_cycles == INS_LDA_IND_X_CYC);
  assert(cpu->A == expected);
  assert(check_lda_flags(cpu, &cpu_copy));
  print_test_pass("LDA_IND_X:", used_cycles);
}
write tests for ind x and ind y
void test_lda_ind_y(CPU *cpu, MEM *mem)
{
  /*
   * The 16 bits + Y register after the instruction are the next address; The go to that address and look at the 8 bits after it
   * that is the value we want in A Again only care for this case page boundary cross
   */
  reset_cpu(cpu, mem);
  u8 add_cycle = 0;
  CPU cpu_copy = *cpu;
  cpu->Y = 0x04;
  u16 zp_addr = 0x0002;
  u16 addr_result = cpu->Y + zp_addr;
  if ((zp_addr >> 8) != (addr_result >> 8)) add_cycle++;
  u16 pc_start = 0XFFFC;
  u8 expected = 0x67;
  mem->Data[pc_start] = INS_LDA_IND_Y;
  mem->Data[0xFFFD] = zp_addr;
  mem->Data[addr_result] = 0x00;
  mem->Data[0x0007] = 0x80;
  mem->Data[0x8000] = expected;
  // Run
  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_IND_Y_CYC);

  // Check
  assert(used_cycles == INS_LDA_IND_Y_CYC + add_cycle);
  assert(cpu->A == expected);
  assert(check_lda_flags(cpu, &cpu_copy));
  print_test_pass("LDA_IND_X:", used_cycles);
}
void test_lda_ind_y_6_cyc(CPU *cpu, MEM *mem)
{
  return;
}
// Output helpers
void print_test_pass(const char *name, s32 used_cycles)
{
  printf("%-*s " COLOR_GREEN "[PASS]" COLOR_RESET " Used " COLOR_YELLOW "%2u" COLOR_RESET " cycles\n", TEST_NAME_WIDTH, name,
         used_cycles);
}