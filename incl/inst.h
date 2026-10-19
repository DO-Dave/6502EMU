#ifndef INST_H
#define INST_H
#include "mem.h"
#define NULL_INS 0x00
// LDA
#define INS_LDA_IM 0xA9
#define INS_LDA_ZP 0xA5
#define INS_LDA_ZP_X 0xB5
#define INS_LDA_ABS 0xAD
#define INS_LDA_ABS_X 0xBD
#define INS_LDA_ABS_Y 0xB9
#define INS_LDA_IND_X 0xA1
#define INS_LDA_IND_Y 0xB1
// STA
#define INS_STA_ZP 0x85
#define INS_STA_ZP_X 0x95
#define INS_STA_ABS 0x8D
#define INS_STA_ABS_X 0x9D
#define INS_STA_ABS_Y 0x99
#define INS_STA_IND_X 0x81
#define INS_STA_IND_Y 0x91
// LDX
#define INS_LDX_IM 0xA2
#define INS_LDX_ZP 0xA6
#define INS_LDX_ZP_Y 0xB6
#define INS_LDX_ABS 0xAE
#define INS_LDX_ABS_Y 0xBE
// LDY
#define INS_LDY_IM 0xA0
#define INS_LDY_ZP 0xA4
#define INS_LDY_ZP_X 0xB4
#define INS_LDY_ABS 0xAC
#define INS_LDY_ABS_X 0xBC
// JSR
#define INS_JSR_A 0x20
// JMP
#define INS_JMP_A 0x4C
#define INS_JMP_IND 0x6C
// cycle count
// LDA
#define INS_LDA_IM_CYC 2
#define INS_LDA_ZP_CYC 3
#define INS_LDA_ZP_X_CYC 4
#define INS_LDA_ABS_CYC 4
#define INS_LDA_ABS_X_CYC 4 // or 4+1 if page bound cross
#define INS_LDA_ABS_Y_CYC 4 // or 4+1 if page bound cross
#define INS_LDA_IND_X_CYC 6
#define INS_LDA_IND_Y_CYC 5 // or 5+1 if page bound cross
// STA
#define INS_STA_ZP_CYC 3
#define INS_STA_ZP_X_CYC 4
#define INS_STA_ABS_CYC 4
#define INS_STA_ABS_X_CYC 5
#define INS_STA_ABS_Y_CYC 5
#define INS_STA_IND_X_CYC 6
#define INS_STA_IND_Y_CYC 6
// LDX
#define INS_LDX_IM_CYC 2
#define INS_LDX_ZP_CYC 3
#define INS_LDX_ZP_Y_CYC 4
#define INS_LDX_ABS_CYC 4
#define INS_LDX_ABS_Y_CYC 4 // +1 if boundary cross
// LDY
#define INS_LDY_IM_CYC 2
#define INS_LDY_ZP_CYC 3
#define INS_LDY_ZP_X_CYC 4
#define INS_LDY_ABS_CYC 4
#define INS_LDY_ABS_X_CYC 4 // +1 if boundary cross
// JSR
#define INS_JSR_A_CYC 6
// JMP
#define INS_JMP_A_CYC 3
#define INS_JMP_IND_CYC 5
u8 cyc_from_opcode(Byte opcode);
#endif // INST_H