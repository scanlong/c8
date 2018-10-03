#include <stdio.h>

#include "SDL.h"
#include "cpu.h"
#include "display.h"

int main(int argc, char* args[]) {
  if (argc != 2) {
    printf("usage: c8 game\n");
    printf("  game: Chip8 binary file to play\n");
    return -1;
  }
  char* filename = args[1];

  struct cpu cpu;
  if (cpu_init(&cpu, filename)) {
    fprintf(stderr, "Unable to initialize the emulator!\n");
    return -1;
  };

  struct display display;
  if (display_init(&display)) {
    fprintf(stderr, "Unable to initialize the display!\n");
    return -1;
  }

  uint32_t start_tick;
  uint32_t frame_speed;
  SDL_Event event;
  bool running = true;
  while (running) {
    start_tick = SDL_GetTicks();

    cpu_cycle(&cpu);
    if (cpu.draw) {
      display_draw(&display, cpu.pixels);
      cpu.draw = false;
    }

    frame_speed = SDL_GetTicks() - start_tick;
    if (frame_speed < MILLISECONDS_PER_CYCLE) {
      SDL_Delay(MILLISECONDS_PER_CYCLE - frame_speed);
    }

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }
  }
  
  display_cleanup(&display);
  return 0;
}
