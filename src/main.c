#include "../incl/cpu.h"
#include "../incl/test.h"
int main()
{
  printf("\n");
  MEM m;
  CPU c;
  CPU *cpu = &c;
  MEM *mem = &m;
  reset_cpu(cpu, mem);

  test_do_nothing(cpu, mem);
  test_garbage_inst(cpu, mem);

  test_exe_full_inst_always_lda_im(cpu, mem);
  test_exe_full_inst_always_lda_zp(cpu, mem);
  test_exe_full_inst_always_lda_zpx(cpu, mem);

  test_lda_ind_x(cpu, mem);
  test_lda_ind_y(cpu, mem);
  test_lda_ind_y_6_cyc(cpu, mem);
  test_load_imm(0x67, &cpu->A, INS_LDA_IM, cpu, mem);
  test_load_imm(0x67, &cpu->X, INS_LDX_IM, cpu, mem);
  test_load_imm(0x67, &cpu->Y, INS_LDY_IM, cpu, mem);

  test_load_zp(0x67, &cpu->A, INS_LDA_ZP, cpu, mem);
  test_load_zp(0x67, &cpu->X, INS_LDX_ZP, cpu, mem);
  test_load_zp(0x67, &cpu->Y, INS_LDY_ZP, cpu, mem);

  test_load_abs(0x67, &cpu->A, INS_LDA_ABS, cpu, mem);
  test_load_abs(0x67, &cpu->X, INS_LDX_ABS, cpu, mem);
  test_load_abs(0x67, &cpu->Y, INS_LDY_ABS, cpu, mem);

  test_load_zp_x(0x67, &cpu->A, &cpu->X, INS_LDA_ZP_X, cpu, mem);
  test_load_zp_x(0x67, &cpu->Y, &cpu->X, INS_LDY_ZP_X, cpu, mem);

  test_load_zp_y(0x67, &cpu->X, &cpu->Y, INS_LDX_ZP_Y, cpu, mem);
  test_load_abs_x(0x67, &cpu->A, &cpu->X, INS_LDA_ABS_X, cpu, mem);
  test_load_abs_x(0x67, &cpu->Y, &cpu->X, INS_LDY_ABS_X, cpu, mem);

  test_load_abs_y(0x67, &cpu->A, &cpu->Y, INS_LDA_ABS_Y, cpu, mem);
  test_load_abs_x(0x67, &cpu->X, &cpu->Y, INS_LDX_ABS_Y, cpu, mem);

  printf("\n");
  return 0;
}
