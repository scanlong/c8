#pragma once
#include "cpu.h"

void cpu_error(struct cpu* cpu);
void cpu_clear(struct cpu* cpu);
void cpu_jump(struct cpu* cpu, uint16_t address);
void cpu_skip(struct cpu* cpu, bool expression);
void cpu_call(struct cpu* cpu, uint16_t address);
void cpu_assign_register(struct cpu* cpu, uint8_t register_id, uint8_t value);
void cpu_add_carry(struct cpu* cpu, uint8_t left, uint8_t right);
void cpu_subtract_borrow(struct cpu* cpu, uint8_t left, uint8_t right);
void cpu_shift_left(struct cpu* cpu);
void cpu_shift_right(struct cpu* cpu);
void cpu_random(struct cpu* cpu);
void cpu_draw(struct cpu* cpu);
void cpu_wait_key_press(struct cpu* cpu);
void cpu_assign_i(struct cpu* cpu, uint16_t value);
void cpu_store_bcd(struct cpu* cpu);
void cpu_copy_to_memory(struct cpu* cpu);
void cpu_copy_from_memory(struct cpu* cpu);
void cpu_assign_delay_timer(struct cpu* cpu, uint8_t value);
void cpu_assign_sound_timer(struct cpu* cpu, uint8_t value);
