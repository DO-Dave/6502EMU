#include "../incl/cpu.h"
#include "../incl/test.h"
int main(void)
{
  /* Static allocation avoids putting the 64 KiB memory array on the stack. */
  static CPU cpu;
  static MEM mem;

  reset_cpu(&cpu, &mem);

  printf("\n========== 6502 TEST SUITE ==========\n\n");

  /* Execution-budget and core instruction tests. */
  test_do_nothing(&cpu, &mem);
  test_exe_full_inst_always_lda_im(&cpu, &mem);
  test_exe_full_inst_always_lda_zp(&cpu, &mem);
  test_exe_full_inst_always_lda_zpx(&cpu, &mem);
  test_exe_full_inst_always_jsr_a(&cpu, &mem);
  test_garbage_inst(&cpu, &mem);
  test_jsr_a(&cpu, &mem);

  /* Indirect LDA addressing tests. */
  test_lda_ind_x(&cpu, &mem);
  test_lda_ind_y(&cpu, &mem);
  test_lda_ind_y_6_cyc(&cpu, &mem);

  /* Immediate loads: LDA #$nn, LDX #$nn, LDY #$nn. */
  test_load_imm(0x67, &cpu.A, INS_LDA_IM, &cpu, &mem);
  test_load_imm(0x00, &cpu.X, INS_LDX_IM, &cpu, &mem);
  test_load_imm(0x80, &cpu.Y, INS_LDY_IM, &cpu, &mem);

  /* Zero-page loads: LDA $nn, LDX $nn, LDY $nn. */
  test_load_zp(0x00, &cpu.A, INS_LDA_ZP, &cpu, &mem);
  test_load_zp(0x80, &cpu.X, INS_LDX_ZP, &cpu, &mem);
  test_load_zp(0x67, &cpu.Y, INS_LDY_ZP, &cpu, &mem);

  /* Zero-page indexed loads. */
  test_load_zp_x(0x80, &cpu.A, &cpu.X, INS_LDA_ZP_X, &cpu, &mem);
  test_load_zp_x(0x00, &cpu.Y, &cpu.X, INS_LDY_ZP_X, &cpu, &mem);
  test_load_zp_y(0x80, &cpu.X, &cpu.Y, INS_LDX_ZP_Y, &cpu, &mem);

  /* Absolute loads. */
  test_load_abs(0x67, &cpu.A, INS_LDA_ABS, &cpu, &mem);
  test_load_abs(0x00, &cpu.X, INS_LDX_ABS, &cpu, &mem);
  test_load_abs(0x80, &cpu.Y, INS_LDY_ABS, &cpu, &mem);

  /* Absolute indexed loads. */
  test_load_abs_x(0x80, &cpu.A, &cpu.X, INS_LDA_ABS_X, &cpu, &mem);
  test_load_abs_x(0x00, &cpu.Y, &cpu.X, INS_LDY_ABS_X, &cpu, &mem);
  test_load_abs_y(0x00, &cpu.A, &cpu.Y, INS_LDA_ABS_Y, &cpu, &mem);
  test_load_abs_y(0x80, &cpu.X, &cpu.Y, INS_LDX_ABS_Y, &cpu, &mem);

  /* JMP absolute and indirect. */
  test_jmp_abs(0x3456, &cpu, &mem);
  test_jmp_ind(0x3000, &cpu, &mem);

  /* STA zero-page and zero-page,X stores. */
  test_store_zp(0x42, 0xA5, &cpu.A, INS_STA_ZP, &cpu, &mem);
  test_store_zp_x(0x42, 0xA5, &cpu.A, &cpu.X, INS_STA_ZP_X, &cpu, &mem);
  test_store_zp_x_wrap(&cpu, &mem);

  printf("\n========== ALL TESTS PASSED ==========\n");
  return 0;
}
