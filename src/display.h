#pragma once
#include "SDL.h"
#include "constant.h"

struct display {
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* texture;
};

int display_init(struct display* display);
int display_check_error(char* name, void* pointer);
void display_cleanup(struct display* display);
void display_draw(struct display* display,
                  uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT]);
