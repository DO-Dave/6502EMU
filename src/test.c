#include "../incl/test.h"

static bool check_load_flags(const CPU *cpu, const CPU *cpu_copy, Byte value)
{
  const bool zero_and_negative_set = (cpu->Z == (value == 0)) && (cpu->N == ((value & 0x80) != 0));

  const bool other_flags_unchanged =
      cpu->C == cpu_copy->C && cpu->I == cpu_copy->I && cpu->D == cpu_copy->D && cpu->B == cpu_copy->B && cpu->V == cpu_copy->V;

  return zero_and_negative_set && other_flags_unchanged;
}

bool check_all_flags_same(const CPU *cpu, const CPU *cpu_copy)
{
  return cpu->C == cpu_copy->C && cpu->Z == cpu_copy->Z && cpu->I == cpu_copy->I && cpu->D == cpu_copy->D &&
         cpu->B == cpu_copy->B && cpu->V == cpu_copy->V && cpu->N == cpu_copy->N;
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

  u16 pc_start = 0xFFFC;
  Byte expected = 0x67;
  cpu->PC = pc_start;
  CPU cpu_copy = *cpu;

  mem->Data[pc_start] = INS_LDA_IM;
  mem->Data[pc_start + 1] = expected;

  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_IM_CYC - 1);

  assert(used_cycles == INS_LDA_IM_CYC);
  assert(cpu->A == expected);
  assert(check_load_flags(cpu, &cpu_copy, expected));

  print_test_pass("EXE_FULL_LDA_IM:", used_cycles);
}

void test_exe_full_inst_always_lda_zp(CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);

  u16 pc_start = 0xFFFC;
  Byte zp_address = 0x42;
  Byte expected = 0x02;
  cpu->PC = pc_start;
  CPU cpu_copy = *cpu;

  mem->Data[pc_start] = INS_LDA_ZP;
  mem->Data[pc_start + 1] = zp_address;
  mem->Data[zp_address] = expected;

  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_ZP_CYC - 1);

  assert(used_cycles == INS_LDA_ZP_CYC);
  assert(cpu->A == expected);
  assert(check_load_flags(cpu, &cpu_copy, expected));

  print_test_pass("EXE_FULL_LDA_ZP:", used_cycles);
}

void test_exe_full_inst_always_lda_zpx(CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);

  u16 pc_start = 0xFFFC;
  Byte zp_offset = 0xFE;
  Byte x_value = 0x05;
  Byte effective_addr = (Byte)(zp_offset + x_value); /* $FE + $05 wraps to $03. */
  Byte expected = 0x03;
  Byte wrong_value = (Byte)(expected ^ 0xFF);

  cpu->PC = pc_start;
  cpu->X = x_value;
  CPU cpu_copy = *cpu;

  mem->Data[pc_start] = INS_LDA_ZP_X;
  mem->Data[pc_start + 1] = zp_offset;
  mem->Data[effective_addr] = expected;
  mem->Data[0x0103] = wrong_value; /* Detect a non-wrapping 16-bit addition. */

  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_ZP_X_CYC - 1);

  assert(used_cycles == INS_LDA_ZP_X_CYC);
  assert(cpu->A == expected);
  assert(check_load_flags(cpu, &cpu_copy, expected));

  print_test_pass("EXE_FULL_LDA_ZPX:", used_cycles);
}

void test_exe_full_inst_always_jsr_a(CPU *cpu, MEM *mem)
{
  /* An instruction that has started must finish, even if it exceeds the cycle budget. */
  reset_cpu(cpu, mem);

  u16 pc_start = 0xFFFC;
  u16 jmp_addr = 0x4342;
  Byte expected = 0x04;
  cpu->PC = pc_start;
  CPU cpu_copy = *cpu;

  mem->Data[pc_start] = INS_JSR_A;
  mem->Data[0xFFFD] = (Byte)(jmp_addr & 0xFF);
  mem->Data[0xFFFE] = (Byte)(jmp_addr >> 8);
  mem->Data[jmp_addr] = INS_LDA_IM;
  mem->Data[jmp_addr + 1] = expected;

  s32 used_cycles = cpu_execute(cpu, mem, INS_JSR_A_CYC + 1);

  assert(used_cycles == INS_JSR_A_CYC + INS_LDA_IM_CYC);
  assert(cpu->A == expected);
  assert(check_all_flags_same(cpu, &cpu_copy));

  print_test_pass("EXE_FULL_JSR_A:", used_cycles);
}

void test_garbage_inst(CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u16 pc_start = 0xFFFC;
  cpu->PC = pc_start;
  mem->Data[pc_start] = NULL_INS;

  s32 used_cycles = cpu_execute(cpu, mem, 5);

  assert(used_cycles == 1);
  assert(check_all_flags_same(cpu, &cpu_copy));

  print_test_pass("GARBAGE_INS:", used_cycles);
}

void test_jsr_a(CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);

  u16 pc_start = 0xFFFC;
  u16 jmp_addr = 0x4342;
  Byte expected = 0x04;
  cpu->PC = pc_start;
  CPU cpu_copy = *cpu;

  mem->Data[pc_start] = INS_JSR_A;
  mem->Data[0xFFFD] = (Byte)(jmp_addr & 0xFF);
  mem->Data[0xFFFE] = (Byte)(jmp_addr >> 8);
  mem->Data[jmp_addr] = INS_LDA_IM;
  mem->Data[jmp_addr + 1] = expected;

  s32 used_cycles = cpu_execute(cpu, mem, INS_JSR_A_CYC + INS_LDA_IM_CYC);

  assert(used_cycles == INS_JSR_A_CYC + INS_LDA_IM_CYC);
  assert(cpu->A == expected);
  assert(check_all_flags_same(cpu, &cpu_copy));

  print_test_pass("JSR_A:", used_cycles);
}

void test_lda_ind_x(CPU *cpu, MEM *mem)
{
  /* LDA ($nn,X): add X to the zero-page operand, read a 16-bit pointer,
     then load A from the address stored in that pointer. */
  reset_cpu(cpu, mem);

  CPU cpu_copy = *cpu;
  cpu->X = 0x04;
  u16 pc_start = 0xFFFC;
  Byte expected = 0x67;

  cpu->PC = pc_start;
  mem->Data[pc_start] = INS_LDA_IND_X;
  mem->Data[pc_start + 1] = 0x02;
  mem->Data[0x0006] = 0x00;
  mem->Data[0x0007] = 0x80;
  mem->Data[0x8000] = expected;

  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_IND_X_CYC);

  assert(used_cycles == INS_LDA_IND_X_CYC);
  assert(cpu->A == expected);
  assert(check_load_flags(cpu, &cpu_copy, expected));

  print_test_pass("LDA_IND_X:", used_cycles);
}

void test_lda_ind_y(CPU *cpu, MEM *mem)
{
  /* LDA ($nn),Y: read a 16-bit address from zero page, add Y,
     and load from the resulting address. No page crossing in this test. */
  reset_cpu(cpu, mem);

  CPU cpu_copy = *cpu;
  cpu->Y = 0x04;

  Byte zp_addr = 0x02;
  u16 base_addr = 0x8002;
  u16 addr_result = (u16)(base_addr + cpu->Y);
  u16 pc_start = 0xFFFC;
  Byte expected = 0x67;

  cpu->PC = pc_start;
  mem->Data[pc_start] = INS_LDA_IND_Y;
  mem->Data[pc_start + 1] = zp_addr;
  mem->Data[0x0002] = (Byte)(base_addr & 0xFF);
  mem->Data[0x0003] = (Byte)(base_addr >> 8);
  mem->Data[addr_result] = expected;

  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_IND_Y_CYC);

  assert((base_addr >> 8) == (addr_result >> 8));
  assert(used_cycles == INS_LDA_IND_Y_CYC);
  assert(cpu->A == expected);
  assert(check_load_flags(cpu, &cpu_copy, expected));

  print_test_pass("LDA_IND_Y:", used_cycles);
}

void test_lda_ind_y_6_cyc(CPU *cpu, MEM *mem)
{
  /* LDA ($nn),Y page-crossing case:
     base = $80FE, Y = $04, effective address = $8102, cycles = 6. */
  reset_cpu(cpu, mem);

  CPU cpu_copy = *cpu;
  cpu->Y = 0x04;

  Byte zp_addr = 0x02;
  u16 base_addr = 0x80FE;
  u16 addr_result = (u16)(base_addr + cpu->Y);
  u16 pc_start = 0xFFFC;
  Byte expected = 0x67;

  cpu->PC = pc_start;
  mem->Data[pc_start] = INS_LDA_IND_Y;
  mem->Data[pc_start + 1] = zp_addr;
  mem->Data[0x0002] = (Byte)(base_addr & 0xFF);
  mem->Data[0x0003] = (Byte)(base_addr >> 8);
  mem->Data[addr_result] = expected;

  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_IND_Y_CYC);

  assert((base_addr >> 8) != (addr_result >> 8));
  assert(used_cycles == INS_LDA_IND_Y_CYC + 1);
  assert(cpu->A == expected);
  assert(check_load_flags(cpu, &cpu_copy, expected));

  print_test_pass("LDA_IND_Y_6_CYC:", used_cycles);
}

void test_load_imm(const Byte expected, Byte *out_reg, const u8 opcode, CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);

  CPU cpu_copy = *cpu;
  u16 pc_start = cpu->PC;
  u8 cycles = cyc_from_opcode(opcode);

  mem->Data[pc_start] = opcode;
  mem->Data[pc_start + 1] = expected;

  s32 used_cycles = cpu_execute(cpu, mem, cycles);

  assert(used_cycles == cycles);
  assert(*out_reg == expected);
  assert(check_load_flags(cpu, &cpu_copy, *out_reg));

  print_test_pass("LOAD IMM", used_cycles);
}

void test_load_zp(const Byte expected, Byte *out_reg, const u8 opcode, CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);

  CPU cpu_copy = *cpu;
  u16 pc_start = cpu->PC;
  Byte zp_address = 0x42;
  u8 cycles = cyc_from_opcode(opcode);

  mem->Data[pc_start] = opcode;
  mem->Data[pc_start + 1] = zp_address; /* One-byte zero-page operand. */
  mem->Data[zp_address] = expected;

  s32 used_cycles = cpu_execute(cpu, mem, cycles);

  assert(used_cycles == cycles);
  assert(*out_reg == expected);
  assert(check_load_flags(cpu, &cpu_copy, *out_reg));

  print_test_pass("LOAD ZP", used_cycles);
}

void test_load_zp_x(const Byte expected, Byte *out_reg, Byte *x_reg, const u8 opcode, CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);

  CPU cpu_copy = *cpu;
  u16 pc_start = cpu->PC;
  Byte zp_offset = 0xFE;
  Byte x_value = 0x05;
  Byte effective_addr = (Byte)(zp_offset + x_value);
  Word unwrapped_addr = (Word)zp_offset + x_value;
  Byte wrong_value = (Byte)(expected ^ 0xFF);
  u8 cycles = cyc_from_opcode(opcode);

  *x_reg = x_value;

  mem->Data[pc_start] = opcode;
  mem->Data[pc_start + 1] = zp_offset;
  mem->Data[effective_addr] = expected;
  mem->Data[unwrapped_addr] = wrong_value;

  s32 used_cycles = cpu_execute(cpu, mem, cycles);

  assert(used_cycles == cycles);
  assert(*out_reg == expected);
  assert(check_load_flags(cpu, &cpu_copy, *out_reg));

  print_test_pass("LOAD ZP X", used_cycles);
}

void test_load_zp_y(const Byte expected, Byte *out_reg, Byte *y_reg, const u8 opcode, CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);

  CPU cpu_copy = *cpu;
  u16 pc_start = cpu->PC;
  Byte zp_offset = 0xFE;
  Byte y_value = 0x05;
  Byte effective_addr = (Byte)(zp_offset + y_value);
  Word unwrapped_addr = (Word)zp_offset + y_value;
  Byte wrong_value = (Byte)(expected ^ 0xFF);
  u8 cycles = cyc_from_opcode(opcode);

  *y_reg = y_value;

  mem->Data[pc_start] = opcode;
  mem->Data[pc_start + 1] = zp_offset;
  mem->Data[effective_addr] = expected;
  mem->Data[unwrapped_addr] = wrong_value;

  s32 used_cycles = cpu_execute(cpu, mem, cycles);

  assert(used_cycles == cycles);
  assert(*out_reg == expected);
  assert(check_load_flags(cpu, &cpu_copy, *out_reg));

  print_test_pass("LOAD ZP Y", used_cycles);
}

void test_load_abs(const Byte expected, Byte *out_reg, const u8 opcode, CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);

  CPU cpu_copy = *cpu;
  u16 pc_start = cpu->PC;
  Word target_addr = 0x4480;
  u8 cycles = cyc_from_opcode(opcode);

  mem->Data[pc_start] = opcode;
  mem->Data[pc_start + 1] = (Byte)(target_addr & 0xFF);
  mem->Data[pc_start + 2] = (Byte)(target_addr >> 8);
  mem->Data[target_addr] = expected;

  s32 used_cycles = cpu_execute(cpu, mem, cycles);

  assert(used_cycles == cycles);
  assert(*out_reg == expected);
  assert(check_load_flags(cpu, &cpu_copy, *out_reg));

  print_test_pass("LOAD ABS", used_cycles);
}

void test_load_abs_x(const Byte expected, Byte *out_reg, Byte *x_reg, const u8 opcode, CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);

  CPU cpu_copy = *cpu;
  u16 pc_start = cpu->PC;
  Word target_addr = 0x4480;
  Byte x_value = 0x01;
  Word result_addr = (Word)(target_addr + x_value);
  u8 cycles = cyc_from_opcode(opcode);

  *x_reg = x_value;
  if ((target_addr >> 8) != (result_addr >> 8)) cycles++;

  mem->Data[pc_start] = opcode;
  mem->Data[pc_start + 1] = (Byte)(target_addr & 0xFF);
  mem->Data[pc_start + 2] = (Byte)(target_addr >> 8);
  mem->Data[result_addr] = expected;

  s32 used_cycles = cpu_execute(cpu, mem, cycles);

  assert(used_cycles == cycles);
  assert(*out_reg == expected);
  assert(check_load_flags(cpu, &cpu_copy, *out_reg));

  print_test_pass("LOAD ABS X", used_cycles);
}

void test_load_abs_y(const Byte expected, Byte *out_reg, Byte *y_reg, const u8 opcode, CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);

  CPU cpu_copy = *cpu;
  u16 pc_start = cpu->PC;
  Word target_addr = 0x4480;
  Byte y_value = 0x01;
  Word result_addr = (Word)(target_addr + y_value);
  u8 cycles = cyc_from_opcode(opcode);

  *y_reg = y_value;
  if ((target_addr >> 8) != (result_addr >> 8)) cycles++;

  mem->Data[pc_start] = opcode;
  mem->Data[pc_start + 1] = (Byte)(target_addr & 0xFF);
  mem->Data[pc_start + 2] = (Byte)(target_addr >> 8);
  mem->Data[result_addr] = expected;

  s32 used_cycles = cpu_execute(cpu, mem, cycles);

  assert(used_cycles == cycles);
  assert(*out_reg == expected);
  assert(check_load_flags(cpu, &cpu_copy, *out_reg));

  print_test_pass("LOAD ABS Y", used_cycles);
}

void test_jmp_abs(const Word target_addr, CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);

  CPU cpu_copy = *cpu;
  u16 pc_start = cpu->PC;
  mem->Data[pc_start] = INS_JMP_A;
  mem->Data[pc_start + 1] = (Byte)(target_addr & 0xFF);
  mem->Data[pc_start + 2] = (Byte)(target_addr >> 8);

  s32 expected_cycles = cyc_from_opcode(INS_JMP_A);
  s32 used_cycles = cpu_execute(cpu, mem, expected_cycles);

  assert(used_cycles == expected_cycles);
  assert(cpu->PC == target_addr);
  assert(check_all_flags_same(cpu, &cpu_copy));

  print_test_pass("JMP ABS", used_cycles);
}

void test_jmp_ind(const Word ind_addr, CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);

  CPU cpu_copy = *cpu;
  u16 pc_start = cpu->PC;
  mem->Data[pc_start] = INS_JMP_IND;
  mem->Data[pc_start + 1] = (Byte)(ind_addr & 0xFF);
  mem->Data[pc_start + 2] = (Byte)(ind_addr >> 8);

  Word target_addr = 0x1234;
  mem->Data[ind_addr] = (Byte)(target_addr & 0xFF);
  mem->Data[ind_addr + 1] = (Byte)(target_addr >> 8);

  s32 expected_cycles = cyc_from_opcode(INS_JMP_IND);
  s32 used_cycles = cpu_execute(cpu, mem, expected_cycles);

  assert(used_cycles == expected_cycles);
  assert(cpu->PC == target_addr);
  assert(check_all_flags_same(cpu, &cpu_copy));

  print_test_pass("JMP IND", used_cycles);
}

/* Store instruction tests. Zero-page operands and target addresses are Bytes. */
void test_store_zp(const Byte zp_addr, const Byte expected, Byte *in_reg, const u8 opcode, CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);

  CPU cpu_copy = *cpu;
  u16 pc_start = cpu->PC;
  u8 cycles = cyc_from_opcode(opcode);
  Byte initial_value = (Byte)(expected ^ 0xFF);

  *in_reg = expected;

  mem->Data[pc_start] = opcode;
  mem->Data[pc_start + 1] = zp_addr; /* Only one operand byte. */
  mem->Data[zp_addr] = initial_value;

  s32 used_cycles = cpu_execute(cpu, mem, cycles);

  assert(used_cycles == cycles);
  assert(mem->Data[zp_addr] == expected);
  assert(check_all_flags_same(cpu, &cpu_copy));

  print_test_pass("STA ZP", used_cycles);
}

void test_store_zp_x(const Byte zp_addr, const Byte expected, Byte *in_reg, Byte *x_reg, const u8 opcode, CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);

  CPU cpu_copy = *cpu;
  u16 pc_start = cpu->PC;
  Byte x_value = 0x04;
  Byte effective_addr = (Byte)(zp_addr + x_value);
  Word unwrapped_addr = (Word)zp_addr + x_value;
  Byte initial_value = (Byte)(expected ^ 0xFF);
  u8 cycles = cyc_from_opcode(opcode);

  *in_reg = expected;
  *x_reg = x_value;

  mem->Data[pc_start] = opcode;
  mem->Data[pc_start + 1] = zp_addr; /* Only one operand byte. */
  mem->Data[effective_addr] = initial_value;

  if (unwrapped_addr > 0x00FF) mem->Data[unwrapped_addr] = initial_value;

  s32 used_cycles = cpu_execute(cpu, mem, cycles);

  assert(used_cycles == cycles);
  assert(mem->Data[effective_addr] == expected);
  assert(check_all_flags_same(cpu, &cpu_copy));

  if (unwrapped_addr > 0x00FF) assert(mem->Data[unwrapped_addr] == initial_value);

  print_test_pass("STA ZP X", used_cycles);
}

void test_store_zp_x_wrap(CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);

  u16 pc_start = cpu->PC;
  Byte expected = 0xA5;
  Byte initial_value = 0x5A;
  Byte zp_offset = 0xFE;
  Byte x_value = 0x04;
  Byte effective_addr = (Byte)(zp_offset + x_value); /* $02 */
  Word unwrapped_addr = (Word)zp_offset + x_value;   /* $0102 */

  cpu->A = expected;
  cpu->X = x_value;
  CPU cpu_copy = *cpu;

  mem->Data[pc_start] = INS_STA_ZP_X;
  mem->Data[pc_start + 1] = zp_offset;
  mem->Data[effective_addr] = initial_value;
  mem->Data[unwrapped_addr] = initial_value;

  s32 expected_cycles = cyc_from_opcode(INS_STA_ZP_X);
  s32 used_cycles = cpu_execute(cpu, mem, expected_cycles);

  assert(used_cycles == expected_cycles);
  assert(mem->Data[effective_addr] == expected);
  assert(mem->Data[unwrapped_addr] == initial_value);
  assert(check_all_flags_same(cpu, &cpu_copy));

  print_test_pass("STA ZP X WRAP", used_cycles);
}

void print_test_pass(const char *name, s32 used_cycles)
{
  printf("%-*s " COLOR_GREEN "[PASS]" COLOR_RESET " Used " COLOR_YELLOW "%2d" COLOR_RESET " cycles\n", TEST_NAME_WIDTH, name,
         used_cycles);
}
