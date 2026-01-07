// MIT License

#ifndef SDL2_SAND_SIMULATION_APP_RENDERER_H_
#define SDL2_SAND_SIMULATION_APP_RENDERER_H_

#include <SDL.h>

#include "window.h"

// Wrapper class for SDL_Renderer (handles its own memory)
// Provides default configuration values
// Provides methods for clearing, rendering and preseting:
// `Clear()`, `RenderFrame(SDL_Texture*)`, `Present()`
// Make sure to use `Ok()` to check if the creation is successful
// Use `Get()` to receive the underlying raw pointer
class Renderer {
 public:
  // Default configuration values, can be overriden in the contructor
  struct Config {
    int32_t index = -1;
    uint32_t flags = SDL_RENDERER_ACCELERATED;
  };

  // Constructor using default configuration values
  Renderer(const Window& window) : Renderer(window, Config{}) {};

  explicit Renderer(const Window& window, const Config&);

  ~Renderer() noexcept;

  // Disallow copies
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  // Allow moves
  Renderer(Renderer&&) noexcept;
  Renderer& operator=(Renderer&&) noexcept;

  bool Ok() const;
  SDL_Renderer* Get() const;

  void Clear();
  void RenderFrame(SDL_Texture* texture);
  void Present();

 private:
  SDL_Renderer* renderer_{nullptr};
};

#endif  // SDL2_SAND_SIMULATION_APP_RENDERER_H_
