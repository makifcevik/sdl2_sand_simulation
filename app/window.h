// MIT License

#ifndef SDL2_SAND_SIMULATION_APP_WINDOW_H_
#define SDL2_SAND_SIMULATION_APP_WINDOW_H_

#include <cstdint>

#include <string>

#include <SDL.h>

// Wrapper class for SDL_Window (handles its own memory)
// Provides default configuration values with default constructor
// Make sure to use `Ok()` to check if the creation is successfull
// Use `Get()` to receive the underlying raw pointer
class Window {
 public:
  // Default configuration values, can be overriden in the contructor
  struct Config {
    std::string title = "Sand Simulation";
    uint32_t width = 1280;
    uint32_t height = 720;
    int32_t x = SDL_WINDOWPOS_CENTERED;
    int32_t y = SDL_WINDOWPOS_CENTERED;
    uint32_t flags = NULL;
  };

  // Default constructor: Allows "Window w;" using defaults
  Window() : Window(Config{}) {}

  explicit Window(const Config&);
  ~Window() noexcept;

  bool Ok() const;
  SDL_Window* Get() const;

 private:
  SDL_Window* window_{nullptr};
  bool ok_{false};
};

#endif  //SDL2_SAND_SIMULATION_APP_WINDOW_H_