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
  // for (u8 expec = 0; expec < 255; expec++)
  //   test_lda_im(cpu, mem, expec);

  test_do_nothing(cpu, mem);
  test_garbage_inst(cpu, mem);

  test_lda_im(cpu, mem, 0x67);
  test_exe_full_inst_always_lda_im(cpu, mem);

  test_lda_zp(cpu, mem);
  test_exe_full_inst_always_lda_zp(cpu, mem);

  test_lda_zpx(cpu, mem);
  test_lda_zpx_wraping(cpu, mem);
  test_exe_full_inst_always_lda_zpx(cpu, mem);

  test_lda_abs(cpu, mem);
  test_lda_abs_x(cpu, mem);
  test_lda_abs_x_5_cyc(cpu, mem);
  test_lda_abs_y(cpu, mem);
  test_lda_abs_y_5_cyc(cpu, mem);

  test_lda_ind_x(cpu, mem);
  test_lda_ind_y(cpu, mem);
  test_lda_ind_y_6_cyc(cpu, mem);

  test_ldx_im(cpu, mem, 0x67);
  test_ldx_zp(cpu, mem);
  test_ldx_zp_y(cpu, mem);
  test_ldx_zp_y_wrapping(cpu, mem);
  test_ldx_abs(cpu, mem);
  test_ldx_abs_y(cpu, mem);
  test_ldx_abs_y_5_cyc(cpu, mem);

  test_ldy_im(cpu, mem, 0x67);
  test_ldy_zp(cpu, mem);
  test_ldy_zp_x(cpu, mem);
  test_ldy_zp_x_wrapping(cpu, mem);
  test_ldy_abs(cpu, mem);
  test_ldy_abs_x(cpu, mem);
  test_ldy_abs_x_5_cyc(cpu, mem);

  printf("\n");
  return 0;
}
