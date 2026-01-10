// MIT License

#include "app.h"

#include <SDL.h>
#include <fmt/core.h>

App::App(const Config& config) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fmt::println(stderr, "Error initializing SDL: {}", SDL_GetError());
    return;  // is_running remains false
  }

  // Create systems
  window_ = std::make_unique<Window>(config.window_config);

  // Only create renderer if window is valid
  if (window_->Ok()) {
    renderer_ = std::make_unique<Renderer>(*window_, config.renderer_config);
  }

  input_ = std::make_unique<Input>();

  if (window_->Ok() && renderer_->Ok()) {
    // Construct texture with default configuration values.
    texture_ = std::make_unique<Texture>(*renderer_);

    // Resize Buffer: Width * Height, filled with Black (0)
    // 0xFF000000 is Black (Alpha=255), 0x00000000 is Transparent
    pixel_buffer_.resize(texture_->GetWidth() * texture_->GetHeight(),
                         0xFF000000);
  }

  // Validation
  // If anything has failed, we are not running
  is_running_ = window_->Ok() && renderer_->Ok() && texture_->Ok();
}

App::~App() noexcept {
  // Destroy resources in REVERSE dependency order.
  // Texture depends on Renderer, destroy it first.
  texture_.reset();

  // Renderer depends on Window.
  renderer_.reset();

  // Window is the root.
  window_.reset();

  // Input is independent, but good to clean up.
  input_.reset();

  // `SDL_Quit()` must be the very last thing called
  SDL_Quit();
}

int App::Run() {
  if (!is_running_)
    return 1;  // Error code

  uint64_t last_time = SDL_GetTicks64();
  double dt{0.0};

  while (is_running_) {
    uint64_t current_time = SDL_GetTicks64();
    uint64_t frame_time = current_time - last_time;
    last_time = current_time;
    dt = static_cast<double>(frame_time) / 1000.0;

    PollEvents();
    Update();
    Render();
  }
  return 0;
}

void App::PollEvents() {
  input_->BeginFrame();
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    // Feed the event to the input system
    input_->ProcessEvent(event);
  }
}

void App::Update() {
  // Update logic here later
  if (input_->QuitRequested()) {
    is_running_ = false;
  }
  if (input_->IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
    is_running_ = false;
  }
}

void App::Render() {
  renderer_->Clear();

  // Test:
  // Generate noise
  if (!pixel_buffer_.empty()) {
    for (uint32_t i = 0; i < pixel_buffer_.size(); ++i) {
      uint8_t random_val = rand() % 255;
      uint32_t color =
          (random_val << 24) | (random_val << 16) | (random_val << 8) | 255;
      pixel_buffer_[i] = color;
    }
    texture_->Update(pixel_buffer_);
  }

  renderer_->RenderFrame(texture_->Get());

  renderer_->Present();
}