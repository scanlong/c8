#pragma once
#include <stdint.h>

union opcode {
  uint16_t instruction;
  struct {
    uint8_t n : 4;
    uint8_t y : 4;
    uint8_t x : 4;
    uint8_t op : 4;
  };
  struct {
    uint8_t kk : 8;
  };
  struct {
    uint16_t addr : 12;
  };
};

enum opcode_type {
  NOT_FOUND,
  CLS,
  RET,
  JP_ADDR,
  CALL_ADDR,
  SE_X_KK,
  SNE_X_KK,
  SE_X_Y,
  LD_X_KK,
  ADD_X_KK,
  LD_X_Y,
  OR_X_Y,
  AND_X_Y,
  XOR_X_Y,
  ADD_X_Y,
  SUB_X_Y,
  SHR_X_Y,
  SUBN_X_Y,
  SHL_X_Y,
  SNE_X_Y,
  LD_I_ADDR,
  JP_0_ADDR,
  RND_X_KK,
  DRW_X_Y_N,
  SKP_X,
  SKNP_X,
  LD_X_DT,
  LD_X_K,
  LD_DT_X,
  LD_ST_X,
  ADD_I_X,
  LD_F_X,
  LD_B_X,
  LD_I_X,
  LD_X_I,
};

enum opcode_type opcode_decode(union opcode* opcode);
