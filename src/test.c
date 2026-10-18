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
  assert(check_load_flags(cpu, &cpu_copy, cpu->A));
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
  assert(check_load_flags(cpu, &cpu_copy, cpu->A));
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
  mem->Data[pc_start] = INS_LDA_ZP_X;
  mem->Data[0xFFFD] = zp_offset;
  mem->Data[zp_offset + x_reg_value] = expected;
  // Run
  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_ZP_X_CYC - 3);
  // Check
  assert(used_cycles == INS_LDA_ZP_X_CYC);
  assert(cpu->A == expected);
  assert(check_load_flags(cpu, &cpu_copy, cpu->A));
  print_test_pass("EXE_FULL_LDA_ZPX:", used_cycles);
}

void test_exe_full_inst_always_jsr_a(CPU *cpu, MEM *mem)
{
  /*
 Important this garantuees that if i start a inst always finishes the no matter if we overshoot the cycles that the cpu was*
 given*/
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
The 16 bits + X register after the instruction are the next address; The go to that address and look at the 8 bits after
it that is the value we want in A*
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
  assert(check_load_flags(cpu, &cpu_copy, cpu->A));
  print_test_pass("LDA_IND_X:", used_cycles);
}

void test_lda_ind_y(CPU *cpu, MEM *mem)
{
  /*
 LDA ($nn),Y:*
 Read a 16-bit address from zero page, add Y,*
 then load A from the resulting address.*
 Do not cross a page boundary.*+
*/
  reset_cpu(cpu, mem);
  u8 add_cycle = 0;
  CPU cpu_copy = *cpu;
  cpu->Y = 0x04;
  u8 zp_addr = 0x02;
  u16 base_addr = 0x8002;
  u16 addr_result = base_addr + cpu->Y;
  if ((base_addr >> 8) != (addr_result >> 8)) add_cycle++;
  u16 pc_start = 0xFFFC;
  u8 expected = 0x67;
  // Instruction and operand
  mem->Data[pc_start] = INS_LDA_IND_Y;
  mem->Data[0xFFFD] = zp_addr;
  // Zero-page pointer contains 0x8002 (little-endian)
  mem->Data[0x0002] = 0x02;
  mem->Data[0x0003] = 0x80;
  // Effective address: 0x8002 + 0x04 = 0x8006
  mem->Data[addr_result] = expected;
  // Run
  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_IND_Y_CYC);
  // Check
  assert(used_cycles == INS_LDA_IND_Y_CYC + add_cycle);
  assert(cpu->A == expected);
  assert(check_load_flags(cpu, &cpu_copy, cpu->A));
  print_test_pass("LDA_IND_Y:", used_cycles);
}

void test_lda_ind_y_6_cyc(CPU *cpu, MEM *mem)
{
  /*
 LDA ($nn),Y:
 Test page-boundary crossing.*
 Base address:      0x80FE*
 Y register:        0x04*
 Effective address: 0x8102*
 Expected cycles:   6*
*/
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  cpu->Y = 0x04;
  u8 zp_addr = 0x02;
  u16 base_addr = 0x80FE;
  u16 addr_result = base_addr + cpu->Y;
  u16 pc_start = 0xFFFC;
  u8 expected = 0x67;
  // Instruction and operand
  mem->Data[pc_start] = INS_LDA_IND_Y;
  mem->Data[0xFFFD] = zp_addr;
  // Zero-page pointer contains 0x80FE (little-endian)
  mem->Data[0x0002] = 0xFE;
  mem->Data[0x0003] = 0x80;
  // Effective address: 0x80FE + 0x04 = 0x8102
  mem->Data[addr_result] = expected;
  // Run
  s32 used_cycles = cpu_execute(cpu, mem, INS_LDA_IND_Y_CYC);
  // Check
  assert((base_addr >> 8) != (addr_result >> 8));
  assert(used_cycles == INS_LDA_IND_Y_CYC + 1);
  assert(cpu->A == expected);
  assert(check_load_flags(cpu, &cpu_copy, cpu->A));
  print_test_pass("LDA_IND_Y_6_CYC:", used_cycles);
}

void test_load_imm(const Byte expec_out, Byte *out_reg, const u8 opcode, CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u8 inst_cycles = cyc_from_opcode(opcode);
  mem->Data[cpu->PC] = opcode;
  mem->Data[cpu->PC + 1] = expec_out;

  s32 used_cycles = cpu_execute(cpu, mem, inst_cycles);

  assert(used_cycles == inst_cycles);
  assert(*out_reg == expec_out);
  assert(check_load_flags(cpu, &cpu_copy, *out_reg));
  print_test_pass("LOAD IMM", used_cycles);
}

void test_load_zp(const Byte expec_out, Byte *out_reg, const u8 opcode, CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u8 inst_cycles = cyc_from_opcode(opcode);
  u16 zp_address = 0x0042;
  mem->Data[cpu->PC] = opcode;
  mem->Data[cpu->PC + 1] = zp_address;
  mem->Data[zp_address] = expec_out;

  s32 used_cycles = cpu_execute(cpu, mem, inst_cycles);
  assert(used_cycles == inst_cycles);
  assert(*out_reg == expec_out);
  assert(check_load_flags(cpu, &cpu_copy, *out_reg));
  print_test_pass("LOAD ZP", used_cycles);
}

void test_load_zp_x(const Byte expec_out, Byte *out_reg, Byte *x_reg, const u8 opcode, CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u8 inst_cycles = cyc_from_opcode(opcode);
  u16 zp_offset = 0x0042;
  u16 x_reg_value = 0x5;
  *x_reg = x_reg_value;
  mem->Data[cpu->PC] = opcode;
  mem->Data[cpu->PC + 1] = zp_offset;
  mem->Data[zp_offset + *x_reg] = expec_out;
  s32 used_cycles = cpu_execute(cpu, mem, inst_cycles);
  assert(used_cycles == inst_cycles);
  assert(*out_reg == expec_out);
  assert(check_load_flags(cpu, &cpu_copy, *out_reg));
  print_test_pass("LOAD ZP X", used_cycles);
}

void test_load_zp_y(const Byte expec_out, Byte *out_reg, Byte *y_reg, const u8 opcode, CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u8 inst_cycles = cyc_from_opcode(opcode);
  u16 zp_offset = 0x0042;
  u16 y_reg_value = 0x5;
  *y_reg = y_reg_value;
  mem->Data[cpu->PC] = opcode;
  mem->Data[cpu->PC + 1] = zp_offset;
  mem->Data[zp_offset + *y_reg] = expec_out;
  s32 used_cycles = cpu_execute(cpu, mem, inst_cycles);
  assert(used_cycles == inst_cycles);
  assert(*out_reg == expec_out);
  assert(check_load_flags(cpu, &cpu_copy, *out_reg));
  print_test_pass("LOAD ZP Y", used_cycles);
}

void test_load_abs(const Byte expec_out, Byte *out_reg, const u8 opcode, CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u8 inst_cycles = cyc_from_opcode(opcode);
  u16 target_addr = 0x4480;
  mem->Data[cpu->PC] = opcode;
  mem->Data[cpu->PC + 1] = target_addr & 0xFF;
  mem->Data[cpu->PC + 2] = (target_addr >> 8) & 0xFF;
  mem->Data[target_addr] = expec_out;

  s32 used_cycles = cpu_execute(cpu, mem, inst_cycles);

  assert(used_cycles == inst_cycles);
  assert(*out_reg == expec_out);
  assert(check_load_flags(cpu, &cpu_copy, *out_reg));
  print_test_pass("LOAD ABS", used_cycles);
}

void test_load_abs_x(const Byte expec_out, Byte *out_reg, Byte *x_reg, const u8 opcode, CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u8 inst_cycles = cyc_from_opcode(opcode);
  u16 target_addr = 0x4480;
  u8 x_val = 0x01;
  *x_reg = x_val;
  u16 result_addr = target_addr + x_val;
  if ((target_addr >> 8) != (result_addr >> 8)) inst_cycles++;
  mem->Data[cpu->PC] = opcode;
  mem->Data[cpu->PC + 1] = target_addr & 0xFF;
  mem->Data[cpu->PC + 2] = (target_addr >> 8) & 0xFF;
  mem->Data[result_addr] = expec_out;

  s32 used_cycles = cpu_execute(cpu, mem, inst_cycles);
  assert(used_cycles == inst_cycles);
  assert(*out_reg == expec_out);
  assert(check_load_flags(cpu, &cpu_copy, *out_reg));
  print_test_pass("LOAD ABS X", used_cycles);
}

void test_load_abs_y(const Byte expec_out, Byte *out_reg, Byte *y_reg, const u8 opcode, CPU *cpu, MEM *mem)
{
  reset_cpu(cpu, mem);
  CPU cpu_copy = *cpu;
  u8 inst_cycles = cyc_from_opcode(opcode);
  u16 target_addr = 0x4480;
  u8 y_val = 0x01;
  *y_reg = y_val;
  u16 result_addr = target_addr + y_val;
  if ((target_addr >> 8) != (result_addr >> 8)) inst_cycles++;
  mem->Data[cpu->PC] = opcode;
  mem->Data[cpu->PC + 1] = target_addr & 0xFF;
  mem->Data[cpu->PC + 2] = (target_addr >> 8) & 0xFF;
  mem->Data[result_addr] = expec_out;

  s32 used_cycles = cpu_execute(cpu, mem, inst_cycles);
  assert(used_cycles == inst_cycles);
  assert(*out_reg == expec_out);
  assert(check_load_flags(cpu, &cpu_copy, *out_reg));
  print_test_pass("LOAD ABS Y", used_cycles);
}

void print_test_pass(const char *name, s32 used_cycles)
{
  printf("%-*s " COLOR_GREEN "[PASS]" COLOR_RESET " Used " COLOR_YELLOW "%2u" COLOR_RESET " cycles\n", TEST_NAME_WIDTH, name,
         used_cycles);
}