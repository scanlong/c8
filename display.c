#include "display.h"

#include <stdio.h>

int display_check_error(char* name, void* pointer) {
  if (!pointer) {
    fprintf(stderr, "Could not create %s: %s\n", name, SDL_GetError());
    return -1;
  }
  return 0;
}

int display_init(struct display* display) {
  SDL_Init(SDL_INIT_VIDEO);

  display->window = SDL_CreateWindow(
      "c8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE, SDL_WINDOW_SHOWN);
  if (display_check_error("window", display->window)) return -1;

  display->renderer =
      SDL_CreateRenderer(display->window, -1, SDL_RENDERER_ACCELERATED);
  if (display_check_error("renderer", display->renderer)) return -1;

  display->texture =
      SDL_CreateTexture(display->renderer, SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
  if (display_check_error("texture", display->window)) return -1;

  return 0;
}

void display_draw(struct display* display,
                  uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT]) {
  SDL_UpdateTexture(display->texture, NULL, pixels,
                    SCREEN_WIDTH * sizeof(uint32_t));
  SDL_RenderCopy(display->renderer, display->texture, NULL, NULL);
  SDL_RenderPresent(display->renderer);
}

void display_cleanup(struct display* display) {
  SDL_DestroyTexture(display->texture);
  SDL_DestroyRenderer(display->renderer);
  SDL_DestroyWindow(display->window);
  SDL_Quit();
}
