// MIT License

#ifndef SDL2_SAND_SIMULATION_APP_TEXTURE_H_
#define SDL2_SAND_SIMULATION_APP_TEXTURE_H_

#include <cstdint>

#include <vector>

#include <SDL.h>

// Forward declaration to avoid circular dependency.
class Renderer;

// Wrapper class for SDL_Texture (handles its own memory).
// Provides default configuration values.
// Provides a method to `Update()` the underlying texture.
// Make sure to use `Ok()` to check if the creation is successful.
// Use `Get()` to receive the underlying raw pointer.
class Texture {
 public:
  struct Config {
    uint32_t width = 320;
    uint32_t height = 180;

    // Standard "one int per pixel" format.
    uint32_t format = SDL_PIXELFORMAT_RGBA8888;

    // MUST BE STREAMING to allow frequent CPU updates.
    int32_t access = SDL_TEXTUREACCESS_STREAMING;

    // Blend mode is NONE to preserve raw pixel information (no blur).
    SDL_BlendMode blend = SDL_BLENDMODE_NONE;
  };

  // Constructor with default configuration values.
  Texture(const Renderer&);
  explicit Texture(const Renderer&, const Config&);

  ~Texture() noexcept;

  // Disallow Copies.
  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;

  // Allow Moves.
  Texture(Texture&&) noexcept;
  Texture& operator=(Texture&&) noexcept;

  // Takes a flat buffer of pixels (width * height) and uploads it to GPU
  void Update(const std::vector<uint32_t>& buffer);

  bool Ok() const { return texture_ != nullptr; }
  SDL_Texture* Get() const { return texture_; }
  uint32_t GetWidth() const { return width_; }
  uint32_t GetHeight() const { return height_; }

 private:
  SDL_Texture* texture_{nullptr};
  uint32_t width_{0};
  uint32_t height_{0};
};

#endif  // SDL2_SAND_SIMULATION_APP_TEXTURE_H_
