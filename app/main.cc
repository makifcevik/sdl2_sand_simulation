#include <SDL.h>
#include <fmt/core.h>

int main(int argc, char* argv[]) {
  fmt::print("Hello, Professional C++ World!\n");

  SDL_Window* window =
      SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_RESIZABLE);
  if (!window)
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

  SDL_DestroyWindow(window);
  return 0;
}