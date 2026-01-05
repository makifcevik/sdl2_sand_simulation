// MIT License

#include "window.h"

#include <fmt/core.h>

// Set `ok_` flag true if window creation is successfull, false otherwise
Window::Window(const Config& config) {
  window_ = SDL_CreateWindow(config.title.c_str(), config.x, config.y, config.width,
                             config.height, config.flags);
  if (!window_) {
    fmt::println(stderr, "Error creating SDL Window: ", SDL_GetError());
    ok_ = false;
  } else {
    ok_ = true;
  }
};
// Destroy only if window is valid
Window::~Window() noexcept {
  if (ok_) {
    SDL_DestroyWindow(window_);
  }
};

bool Window::Ok() const {
  return ok_;
};
SDL_Window* Window::Get() const {
  return window_;
}