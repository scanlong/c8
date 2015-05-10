#include "opcode.h"

enum opcode_type opcode_decode(union opcode* opcode) {
  switch (opcode->op) {
    case 0x0:
      switch (opcode->n) {
        case 0x0: return CLS;
        case 0xE: return RET;
      }
    case 0x1: return JP_ADDR;
    case 0x2: return CALL_ADDR;
    case 0x3: return SE_X_KK;
    case 0x4: return SNE_X_KK;
    case 0x5: return SE_X_Y;
    case 0x6: return LD_X_KK;
    case 0x7: return ADD_X_KK;
    case 0x8:
      switch (opcode->n) {
        case 0x0: return LD_X_Y;
        case 0x1: return OR_X_Y;
        case 0x2: return AND_X_Y;
        case 0x3: return XOR_X_Y;
        case 0x4: return ADD_X_Y;
        case 0x5: return SUB_X_Y;
        case 0x6: return SHR_X_Y;
        case 0x7: return SUBN_X_Y;
        case 0xE: return SHL_X_Y;
      }
    case 0x9: return SNE_X_Y;
    case 0xA: return LD_I_ADDR;
    case 0xB: return JP_0_ADDR;
    case 0xC: return RND_X_KK;
    case 0xD: return DRW_X_Y_N;
    case 0xE:
      switch (opcode->kk) {
        case 0x9E: return SKP_X;
        case 0xA1: return SKNP_X;
      }
    case 0xF:
      switch (opcode->kk) {
        case 0x07: return LD_X_DT;
        case 0x0A: return LD_X_K;
        case 0x15: return LD_DT_X;
        case 0x18: return LD_ST_X;
        case 0x1E: return ADD_I_X;
        case 0x29: return LD_F_X;
        case 0x33: return LD_B_X;
        case 0x55: return LD_I_X;
        case 0x65: return LD_X_I;
      }
    default: return NOT_FOUND;
  }
}
