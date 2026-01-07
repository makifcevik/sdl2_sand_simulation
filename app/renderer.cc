// MIT License

#include "renderer.h"

#include <utility>

#include <fmt/core.h>

Renderer::Renderer(const Window& window, const Config& config) {
  renderer_ = SDL_CreateRenderer(window.Get(), config.index, config.flags);
  if (!renderer_) {
    fmt::println(stderr, "Error creating renderer: {}", SDL_GetError());
  }
}

// Move constructor
Renderer::Renderer(Renderer&& other) noexcept
    : renderer_(std::exchange(other.renderer_, nullptr)) {}

// Move assignment
Renderer& Renderer::operator=(Renderer&& other) noexcept {
  if (this != &other) {
    // Destroy current renderer before taking a new one
    if (renderer_) {
      SDL_DestroyRenderer(renderer_);
    }
    renderer_ = std::exchange(other.renderer_, nullptr);
  }
  return *this;
}

// Destroy only if renderer is valid
Renderer::~Renderer() noexcept {
  if (renderer_) {
    SDL_DestroyRenderer(renderer_);
  }
}

bool Renderer::Ok() const {
  return renderer_ != nullptr;
}
SDL_Renderer* Renderer::Get() const {
  return renderer_;
}

// Rending methods
void Renderer::Clear() {
  SDL_RenderClear(renderer_);
}
void Renderer::RenderFrame(SDL_Texture* texture) {
  SDL_RenderCopy(renderer_, texture, nullptr, nullptr);
}
void Renderer::Present() {
  SDL_RenderPresent(renderer_);
}