#ifndef TEST_H
#define TEST_H
#include "cpu.h"

#include <stdbool.h>
#include <stdio.h>
#define TEST_NAME_WIDTH 22
/* General Tests */
void test_do_nothing(CPU *cpu, MEM *mem);
void test_garbage_inst(CPU *cpu, MEM *mem);

/* LDA Instruction */
void test_lda_im(CPU *cpu, MEM *mem, u8 expected);
void test_lda_zp(CPU *cpu, MEM *mem);
void test_lda_zpx(CPU *cpu, MEM *mem);
void test_lda_zpx_wraping(CPU *cpu, MEM *mem);
void test_lda_abs(CPU *cpu, MEM *mem);
void test_lda_abs_x(CPU *cpu, MEM *mem);
void test_lda_abs_x_5_cyc(CPU *cpu, MEM *mem);
void test_lda_abs_y(CPU *cpu, MEM *mem);
void test_lda_abs_y_5_cyc(CPU *cpu, MEM *mem);
void test_lda_ind_x(CPU *cpu, MEM *mem);
void test_lda_ind_y(CPU *cpu, MEM *mem);
void test_lda_ind_y_6_cyc(CPU *cpu, MEM *mem);
/* LDX Instruction */
void test_ldx_im(CPU *cpu, MEM *mem, u8 expected);
void test_ldx_zp(CPU *cpu, MEM *mem);
void test_ldx_zp_y(CPU *cpu, MEM *mem);
void test_ldx_zp_y_wrapping(CPU *cpu, MEM *mem);
void test_ldx_abs(CPU *cpu, MEM *mem);
void test_ldx_abs_y(CPU *cpu, MEM *mem);
void test_ldx_abs_y_5_cyc(CPU *cpu, MEM *mem);
/* LDY Instruction*/
void test_ldy_im(CPU *cpu, MEM *mem, u8 expected);
void test_ldy_zp(CPU *cpu, MEM *mem);
void test_ldy_zp_x(CPU *cpu, MEM *mem);
void test_ldy_zp_x_wrapping(CPU *cpu, MEM *mem);
void test_ldy_abs(CPU *cpu, MEM *mem);
void test_ldy_abs_x(CPU *cpu, MEM *mem);
void test_ldy_abs_x_5_cyc(CPU *cpu, MEM *mem);

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

#endif /* TEST_H */