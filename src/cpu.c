#include "cpu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "SDL.h"
#include "constant.h"
#include "instruction.h"

int cpu_init(struct cpu* cpu, char* filename) {
  FILE* game = fopen(filename, "rb");
  if (!game) {
    fprintf(stderr, "Unable to open file '%s'!\n", filename);
    return -1;
  }
  fread(&cpu->memory[PC_START], 1, MEMORY_SIZE - PC_START, game);
  fclose(game);

  memcpy(cpu->memory, font, sizeof(font));
  memset(cpu->pixels, OFF_COLOR, sizeof(cpu->pixels));
  memset(cpu->stack, 0, sizeof(cpu->stack));
  memset(cpu->v, 0, sizeof(cpu->v));
  memset(cpu->keys, 0, sizeof(cpu->keys));

  cpu->pc = PC_START;
  cpu->opcode.instruction = 0;
  cpu->i = 0;
  cpu->sp = 0;
  cpu->draw = 0;
  cpu->sound_timer = 0;
  cpu->delay_timer = 0;

  srand(time(NULL));
  return 0;
}

void cpu_fetch(struct cpu* cpu) {
  cpu->opcode.instruction =
      cpu->memory[cpu->pc] << 8 | cpu->memory[cpu->pc + 1];
}

void cpu_update_timers(struct cpu* cpu) {
  if (cpu->delay_timer > 0) cpu->delay_timer--;
  if (cpu->sound_timer > 0) cpu->sound_timer--;
}

void cpu_cycle(struct cpu* cpu) {
  cpu_fetch(cpu);
  cpu_execute(cpu);
  cpu_update_timers(cpu);
}

void cpu_execute(struct cpu* cpu) {
  cpu->pc += 2;
  uint8_t vx = cpu->v[cpu->opcode.x];
  uint8_t vy = cpu->v[cpu->opcode.y];
  switch (cpu->opcode.op) {
    case 0x0:
      switch (cpu->opcode.n) {
        case 0x0: return cpu_clear(cpu);
        case 0xE: return cpu_jump(cpu, cpu->stack[--cpu->sp]);
        default:  return cpu_error(cpu);
      }
    case 0x1: return cpu_jump(cpu, cpu->opcode.addr);
    case 0x2: return cpu_call(cpu, cpu->opcode.addr);
    case 0x3: return cpu_skip(cpu, vx == cpu->opcode.kk);
    case 0x4: return cpu_skip(cpu, vx != cpu->opcode.kk);
    case 0x5: return cpu_skip(cpu, vx == vy);
    case 0x6: return cpu_assign_register(cpu, cpu->opcode.x, cpu->opcode.kk);
    case 0x7: return cpu_assign_register(cpu, cpu->opcode.x, vx + cpu->opcode.kk);
    case 0x8:
      switch (cpu->opcode.n) {
        case 0x0: return cpu_assign_register(cpu, cpu->opcode.x, vy);
        case 0x1: return cpu_assign_register(cpu, cpu->opcode.x, vx | vy);
        case 0x2: return cpu_assign_register(cpu, cpu->opcode.x, vx & vy);
        case 0x3: return cpu_assign_register(cpu, cpu->opcode.x, vx ^ vy);
        case 0x4: return cpu_add_carry(cpu, vx, vy);
        case 0x5: return cpu_subtract_borrow(cpu, vx, vy);
        case 0x6: return cpu_shift_right(cpu);
        case 0x7: return cpu_subtract_borrow(cpu, vy, vx);
        case 0xE: return cpu_shift_left(cpu);
        default:  return cpu_error(cpu);
      }
    case 0x9: return cpu_skip(cpu, vx != vy);
    case 0xA: return cpu_assign_i(cpu, cpu->opcode.addr);
    case 0xB: return cpu_jump(cpu, cpu->opcode.addr + cpu->v[0]);
    case 0xC: return cpu_random(cpu);
    case 0xD: return cpu_draw(cpu);
    case 0xE:
      switch (cpu->opcode.kk) {
        case 0x9E: return cpu_skip(cpu, SDL_GetKeyboardState(NULL)[key_map[vx]]);
        case 0xA1: return cpu_skip(cpu, !SDL_GetKeyboardState(NULL)[key_map[vx]]);
        default:   return cpu_error(cpu);
      }
    case 0xF:
      switch (cpu->opcode.kk) {
        case 0x07: return cpu_assign_register(cpu, cpu->opcode.x, cpu->delay_timer);
        case 0x0A: return cpu_wait_key_press(cpu);
        case 0x15: return cpu_assign_delay_timer(cpu, vx);
        case 0x18: return cpu_assign_sound_timer(cpu, vx);
        case 0x1E: return cpu_assign_i(cpu, cpu->i + vx);
        case 0x29: return cpu_assign_i(cpu, vx * 5);
        case 0x33: return cpu_store_bcd(cpu);
        case 0x55: return cpu_copy_to_memory(cpu);
        case 0x65: return cpu_copy_from_memory(cpu);
        default:   return cpu_error(cpu);
      }
    }
  return cpu_error(cpu);
}
