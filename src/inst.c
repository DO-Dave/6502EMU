#include "../incl/inst.h"

u8 cyc_from_opcode(Byte opcode)
{
  switch (opcode)
  {
    case INS_LDA_IM:
      return INS_LDA_IM_CYC;
    case INS_LDA_ZP:
      return INS_LDA_ZP_CYC;
    case INS_LDA_ZP_X:
      return INS_LDA_ZP_X_CYC;
    case INS_JSR_A:
      return INS_JSR_A_CYC;
    case INS_LDA_ABS:
      return INS_LDA_ABS_CYC;
    case INS_LDA_ABS_X:
      return INS_LDA_ABS_X_CYC;
    case INS_LDA_ABS_Y:
      return INS_LDA_ABS_Y_CYC;
    case INS_LDA_IND_X:
      return INS_LDA_IND_X_CYC;
    case INS_LDA_IND_Y:
      return INS_LDA_IND_Y_CYC;
    case INS_LDX_IM:
      return INS_LDX_IM_CYC;
    case INS_LDX_ZP:
      return INS_LDX_ZP_CYC;
    case INS_LDX_ZP_Y:
      return INS_LDX_ZP_Y_CYC;
    case INS_LDX_ABS:
      return INS_LDX_ABS_CYC;
    case INS_LDX_ABS_Y:
      return INS_LDX_ABS_Y_CYC;
    case INS_LDY_IM:
      return INS_LDY_IM_CYC;
    case INS_LDY_ZP:
      return INS_LDY_ZP_CYC;
    case INS_LDY_ZP_X:
      return INS_LDY_ZP_X_CYC;
    case INS_LDY_ABS:
      return INS_LDY_ABS_CYC;
    case INS_LDY_ABS_X:
      return INS_LDY_ABS_X_CYC;
    default:
      return (u8)-1;
  }
}