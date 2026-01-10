// MIT License

#include "texture.h"

#include <utility>

#include <fmt/core.h>

#include "renderer.h"

Texture::Texture(const Renderer& renderer) : Texture(renderer, Config{}) {}

Texture::Texture(const Renderer& renderer, const Config& config)
    : width_(config.width), height_(config.height) {

  texture_ = SDL_CreateTexture(renderer.Get(), config.format, config.access,
                               config.width, config.height);
  if (!texture_) {
    fmt::println(stderr, "Error creating texture: {}", SDL_GetError());
  } else {
    SDL_SetTextureBlendMode(texture_, config.blend);
  }
}

Texture::~Texture() noexcept {
  if (texture_) {
    SDL_DestroyTexture(texture_);
  }
}

// Moves Constructor
Texture::Texture(Texture&& other) noexcept
    : texture_(std::exchange(other.texture_, nullptr)),
      width_(other.width_),
      height_(other.height_) {}

// Move Assignment
Texture& Texture::operator=(Texture&& other) noexcept {
  if (this != &other) {
    // Free current resources
    if (texture_) {
      SDL_DestroyTexture(texture_);
    }

    // Acquire new resources
    texture_ = std::exchange(other.texture_, nullptr);
    width_ = other.width_;
    height_ = other.height_;
  }
  return *this;
}

// Takes an array of CPU pixels and uploads them to the GPU. 
void Texture::Update(const std::vector<uint32_t>& buffer) {
  if (!texture_)
    return;

  void* pixels;
  int pitch;

  //  Lock the texture to get write access to the GPU memory.
  // 'pitch' will return the width of one row in bytes (including padding).
  if (SDL_LockTexture(texture_, nullptr, &pixels, &pitch) != 0)
    return;  // Lock failed.

  // Cast void* pixels to uint8_t* for byte-level pointer arithmetics.
  uint8_t* const dest_ptr = static_cast<uint8_t*>(pixels);
  const uint32_t* const src_ptr = buffer.data();

  const uint32_t row_byte_size = width_ * sizeof(uint32_t);

  // Copy row-by-row to handle potential GPU padding (pitch != width).
  for (int row = 0; row < height_; ++row) {
    // Destination: Start of GPU row.
    uint8_t* dest_row = dest_ptr + (row * pitch);

    // Source: Start of CPU row.
    const uint32_t* src_row = src_ptr + (row * width_);

    memcpy(dest_row, src_row, row_byte_size);
  }

  // Unlock and upload changes to GPU;
  SDL_UnlockTexture(texture_);
}