// MIT License

#include "window.h"

#include <utility>

#include <fmt/core.h>

Window::Window(const Config& config) {
  window_ = SDL_CreateWindow(config.title.c_str(), config.x, config.y,
                             config.width, config.height, config.flags);
  if (!window_) {
    fmt::println(stderr, "Error creating SDL Window: {}", SDL_GetError());
  }
  width_ = config.width;
  height_ = config.height;
};

// Move constructor
Window::Window(Window&& other) noexcept
    : width_(other.width_),
      height_(other.height_),
      window_(std::exchange(other.window_, nullptr)) {}

// Move assignment
Window& Window::operator=(Window&& other) noexcept {
  if (this != &other) {
    // Destroy current window before taking the new one
    if (window_) {
      SDL_DestroyWindow(window_);
    }
    width_ = other.width_;
    height_ = other.height_;
    window_ = std::exchange(other.window_, nullptr);
  }
  return *this;
}

// Destroy only if window is valid
Window::~Window() noexcept {
  if (window_) {
    SDL_DestroyWindow(window_);
  }
};

bool Window::Ok() const {
  return window_ != nullptr;
};
SDL_Window* Window::Get() const {
  return window_;
}