#include "instruction.h"

#include "SDL.h"
#include "constant.h"

void cpu_error(struct cpu* cpu) {
  fprintf(stderr, "ERROR: %x at pc %u could not be executed.\n",
          cpu->opcode.instruction, cpu->pc);
}

void cpu_clear(struct cpu* cpu) {
  memset(cpu->pixels, OFF_COLOR, sizeof(cpu->pixels));
}

void cpu_jump(struct cpu* cpu, uint16_t address) { cpu->pc = address; }

void cpu_skip(struct cpu* cpu, bool expression) {
  if (expression) {
    cpu->pc += 2;
  }
}

void cpu_call(struct cpu* cpu, uint16_t address) {
  cpu->stack[cpu->sp++] = cpu->pc;
  cpu_jump(cpu, address);
}

void cpu_assign_register(struct cpu* cpu, uint8_t register_id, uint8_t value) {
  cpu->v[register_id] = value;
}

void cpu_add_carry(struct cpu* cpu, uint8_t left, uint8_t right) {
  cpu->v[CARRY_REGISTER] = left + right > 255;
  cpu->v[cpu->opcode.x] = left + right;
}

void cpu_subtract_borrow(struct cpu* cpu, uint8_t left, uint8_t right) {
  cpu->v[CARRY_REGISTER] = left > right;
  cpu->v[cpu->opcode.x] = left - right;
}

void cpu_shift_left(struct cpu* cpu) {
  cpu->v[CARRY_REGISTER] = (bool)(cpu->v[cpu->opcode.x] & 0x80);
  cpu->v[cpu->opcode.x] <<= 2;
}

void cpu_shift_right(struct cpu* cpu) {
  cpu->v[CARRY_REGISTER] = cpu->v[cpu->opcode.x] & 1;
  cpu->v[cpu->opcode.x] >>= 2;
}

void cpu_random(struct cpu* cpu) {
  cpu->v[cpu->opcode.x] = (rand() % 256) & cpu->opcode.kk;
}

void cpu_draw(struct cpu* cpu) {
  cpu->v[CARRY_REGISTER] = 0;
  for (int y = 0; y < cpu->opcode.n; y++) {
    for (int x = 0; x < 8; x++) {
      uint8_t pixel = cpu->memory[cpu->i + y];
      if (pixel & (0x80 >> x)) {
        int index =
            (cpu->v[cpu->opcode.x] + x) % SCREEN_WIDTH +
            ((cpu->v[cpu->opcode.y] + y) % SCREEN_HEIGHT) * SCREEN_WIDTH;
        if (cpu->pixels[index] == ON_COLOR) {
          cpu->v[CARRY_REGISTER] = 1;
          cpu->pixels[index] = OFF_COLOR;
        } else {
          cpu->pixels[index] = ON_COLOR;
        }
        cpu->draw = true;
      }
    }
  }
}

void cpu_wait_key_press(struct cpu* cpu) {
  cpu->pc -= 2;
  for (int i = 0; i < 16; i++) {
    if (SDL_GetKeyboardState(NULL)[key_map[i]]) {
      cpu->v[cpu->opcode.x] = i;
      cpu->pc += 2;
      break;
    }
  }
}

void cpu_assign_i(struct cpu* cpu, uint16_t value) {
  cpu->i = value;
}

void cpu_store_bcd(struct cpu* cpu) {
  cpu->memory[cpu->i]     = cpu->v[cpu->opcode.x] / 100;
  cpu->memory[cpu->i + 1] = (cpu->v[cpu->opcode.x] / 10) % 10;
  cpu->memory[cpu->i + 2] = cpu->v[cpu->opcode.x] % 10;
}

void cpu_copy_to_memory(struct cpu* cpu) {
  memcpy(cpu->memory + cpu->i, cpu->v, cpu->opcode.x + 1);
}

void cpu_copy_from_memory(struct cpu* cpu) {
  memcpy(cpu->v, cpu->memory + cpu->i, cpu->opcode.x + 1);
}

void cpu_assign_delay_timer(struct cpu* cpu, uint8_t value) {
  cpu->delay_timer = value;
}

void cpu_assign_sound_timer(struct cpu* cpu, uint8_t value) {
  cpu->sound_timer = value;
}
