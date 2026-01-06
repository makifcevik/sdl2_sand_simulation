// MIT License

#include <SDL.h>
#include <fmt/core.h>

#include "window.h"

int main(int argc, char* argv[]) {
  fmt::print("Hello, Professional C++ World!\n");

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fmt::println(stderr, "Error initializing SDL: {}", SDL_GetError());
  }

  Window window;
  if (!window.Get())
    return 1;

  bool is_running = true;
  SDL_Event event;
  while (is_running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        is_running = false;
      }
    }
  }

  return 0;
}