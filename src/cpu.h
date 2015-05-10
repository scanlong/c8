#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "constant.h"
#include "opcode.h"

struct cpu {
  union opcode opcode;
  uint8_t memory[MEMORY_SIZE];
  uint8_t v[16];
  uint16_t i;
  uint16_t pc;

  uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
  bool keys[16];

  bool draw;

  uint16_t stack[16];
  uint16_t sp;

  uint16_t sound_timer;
  uint16_t delay_timer;
};

int cpu_init(struct cpu* cpu, char* filename);
void cpu_fetch(struct cpu* cpu);
void cpu_execute(struct cpu* cpu);
void cpu_update_timers(struct cpu* cpu);
void cpu_cycle(struct cpu* cpu);

void cpu_skip(struct cpu* cpu, bool expression);
