#ifndef TEST_H
#define TEST_H
#include "cpu.h"
#include "inst.h"

#include <stdbool.h>
#include <stdio.h>
#define TEST_NAME_WIDTH 22
/* General Tests */
void test_do_nothing(CPU *cpu, MEM *mem);
void test_garbage_inst(CPU *cpu, MEM *mem);

/* LDA Instruction */

void test_lda_ind_x(CPU *cpu, MEM *mem);
void test_lda_ind_y(CPU *cpu, MEM *mem);
void test_lda_ind_y_6_cyc(CPU *cpu, MEM *mem);
/* LDX Instruction */
/* LDY Instruction*/

/* JSR Instruction */
void test_jsr_a(CPU *cpu, MEM *mem);

/* Full Execution */
void test_exe_full_inst_always_lda_im(CPU *cpu, MEM *mem);
void test_exe_full_inst_always_lda_zp(CPU *cpu, MEM *mem);
void test_exe_full_inst_always_lda_zpx(CPU *cpu, MEM *mem);
void test_exe_full_inst_always_jsr_a(CPU *cpu, MEM *mem);

/* Status Flags */
bool check_lda_flags(const CPU *cpu, const CPU *cpu_copy);
bool check_all_flags_same(const CPU *cpu, const CPU *cpu_copy);

/* Test Utilities */
void print_test_pass(const char *name, s32 used_cycles);

void test_load_imm(const Byte expec_out, Byte *out_reg, const u8 opcode, CPU *cpu, MEM *mem);

void test_load_zp(const Byte expec_out, Byte *out_reg, const u8 opcode, CPU *cpu, MEM *mem);

void test_load_zp_x(const Byte expec_out, Byte *out_reg, Byte *x_reg, const u8 opcode, CPU *cpu, MEM *mem);

void test_load_zp_y(const Byte expec_out, Byte *out_reg, Byte *y_reg, const u8 opcode, CPU *cpu, MEM *mem);

void test_load_abs(const Byte expec_out, Byte *out_reg, const u8 opcode, CPU *cpu, MEM *mem);

void test_load_abs_x(const Byte expec_out, Byte *out_reg, Byte *x_reg, const u8 opcode, CPU *cpu, MEM *mem);

void test_load_abs_y(const Byte expec_out, Byte *out_reg, Byte *y_reg, const u8 opcode, CPU *cpu, MEM *mem);

void test_jmp_abs(const Word target_addr, CPU *cpu, MEM *mem);

void test_jmp_abs(const Word target_addr, CPU *cpu, MEM *mem);

void test_jmp_ind(const Word ind_addr, CPU *cpu, MEM *mem);
void test_store_zp(const Byte zp_addr, const Byte expected, Byte *in_reg, const u8 opcode, CPU *cpu, MEM *mem);
void test_store_zp_x(const Byte zp_addr, const Byte expected, Byte *in_reg, Byte *x_reg, const u8 opcode, CPU *cpu, MEM *mem);
void test_store_zp_x_wrap(CPU *cpu, MEM *mem);
#endif /* TEST_H */