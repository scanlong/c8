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
