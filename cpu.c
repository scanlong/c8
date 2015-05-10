#include "cpu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "SDL.h"

const uint8_t font[80] = {
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

const SDL_Scancode key_map[16] = {
    SDL_SCANCODE_1,
    SDL_SCANCODE_2,
    SDL_SCANCODE_3,
    SDL_SCANCODE_4,
    SDL_SCANCODE_Q,
    SDL_SCANCODE_W,
    SDL_SCANCODE_E,
    SDL_SCANCODE_R,
    SDL_SCANCODE_A,
    SDL_SCANCODE_S,
    SDL_SCANCODE_D,
    SDL_SCANCODE_F,
    SDL_SCANCODE_Z,
    SDL_SCANCODE_X,
    SDL_SCANCODE_C,
    SDL_SCANCODE_V
};

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
  cpu->opcode.instruction = cpu->memory[cpu->pc] << 8 | cpu->memory[cpu->pc + 1];
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

void cpu_skip(struct cpu* cpu, bool expression) {
  if (expression) {
    cpu->pc += 2;
  }
}

void cpu_execute(struct cpu* cpu) {
  enum opcode_type opcode_type = opcode_decode(&cpu->opcode);
  cpu->pc += 2;

  uint8_t* vx = &cpu->v[cpu->opcode.x];
  uint8_t* vy = &cpu->v[cpu->opcode.y];

  switch (opcode_type) {
    case CLS:
      memset(cpu->pixels, false, sizeof(cpu->pixels));
      break;
    case RET:
      cpu->pc = cpu->stack[--cpu->sp];
      break;
    case JP_ADDR:
      cpu->pc = cpu->opcode.addr;
      break;
    case CALL_ADDR:
      cpu->stack[cpu->sp++] = cpu->pc;
      cpu->pc = cpu->opcode.addr;
      break;
    case SE_X_KK:
      cpu_skip(cpu, *vx == cpu->opcode.kk);
      break;
    case SNE_X_KK:
      cpu_skip(cpu, *vx != cpu->opcode.kk);
      break;
    case SE_X_Y:
      cpu_skip(cpu, *vx == *vy);
      break;
    case LD_X_KK:
      *vx = cpu->opcode.kk;
      break;
    case ADD_X_KK:
      *vx += cpu->opcode.kk;
      break;
    case LD_X_Y:
      *vx = *vy;
      break;
    case OR_X_Y:
      *vx |= *vy;
      break;
    case AND_X_Y:
      *vx &= *vy;
      break;
    case XOR_X_Y:
      *vx ^= *vy;
      break;
    case ADD_X_Y: {
      uint16_t result = *vx + *vy;
      cpu->v[0xF] = result > 255;
      *vx = (uint8_t)result;
      break;
    }
    case SUB_X_Y:
      cpu->v[0xF] = *vx > *vy;
      *vx -= *vy;
      break;
    case SHR_X_Y:
      cpu->v[0xF] = *vx & 1;
      *vx >>= 2;
      break;
    case SUBN_X_Y:
      cpu->v[0xF] = *vy > *vx;
      *vx = *vy - *vx;
      break;
    case SHL_X_Y:
      cpu->v[0xF] = (bool)(*vx & 0x80);
      *vx <<= 2;
      break;
    case SNE_X_Y:
      cpu_skip(cpu, *vx != *vy);
      break;
    case LD_I_ADDR:
      cpu->i = cpu->opcode.addr;
      break;
    case JP_0_ADDR:
      cpu->pc = cpu->opcode.addr + cpu->v[0];
      break;
    case RND_X_KK:
      *vx = (rand() % 256) & cpu->opcode.kk;
      break;
    case DRW_X_Y_N:
      cpu->v[0xF] = 0;
      for (int y = 0; y < cpu->opcode.n; y++) {
        for (int x = 0; x < 8; x++) {
          uint8_t pixel = cpu->memory[cpu->i + y];
          if (pixel & (0x80 >> x)) {
            int index = (*vx + x) % SCREEN_WIDTH +
                        ((*vy + y) % SCREEN_HEIGHT) * SCREEN_WIDTH;
            if (cpu->pixels[index] == ON_COLOR) {
              cpu->v[0xF] = 1;
              cpu->pixels[index] = OFF_COLOR;
            } else cpu->pixels[index] = ON_COLOR;
            cpu->draw = true;
          }
        }
      }
      break;
    case SKP_X:
      cpu_skip(cpu, SDL_GetKeyboardState(NULL)[key_map[*vx]]);
      break;
    case SKNP_X:
      cpu_skip(cpu, !SDL_GetKeyboardState(NULL)[key_map[*vx]]);
      break;
    case LD_X_DT:
      *vx = cpu->delay_timer;
      break;
    case LD_X_K:
      cpu->pc -= 2;
      for (int i = 0; i < 16; i++) {
        if (SDL_GetKeyboardState(NULL)[key_map[i]]) {
          *vx = i;
          cpu->pc += 2;
          break;
        }
      }
      break;
    case LD_DT_X:
      cpu->delay_timer = *vx;
    case LD_ST_X:
      cpu->sound_timer = *vx;
      break;
    case ADD_I_X:
      cpu->i += *vx;
      break;
    case LD_F_X:
      cpu->i = 5 * *vx;
      break;
    case LD_B_X:
      cpu->memory[cpu->i] = *vx / 100;
      cpu->memory[cpu->i + 1] = (*vx / 10) % 10;
      cpu->memory[cpu->i + 2] = *vx % 10;
      break;
    case LD_I_X:
      for (int i = 0; i <= cpu->opcode.x; i++) {
        cpu->memory[cpu->i + i] = cpu->v[i];
      }
      break;
    case LD_X_I:{
      for (int i = 0; i <= cpu->opcode.x; i++) {
        cpu->v[i] = cpu->memory[cpu->i + i];
      }
      break;
    }
    default: {
      fprintf(stderr, "ERROR: %x at pc %u could not be executed.\n",
              cpu->opcode.instruction, cpu->pc);
      break;
    }
  }
}
