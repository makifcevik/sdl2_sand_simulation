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

  // Validation
  // If anything has failed, we are not running
  is_running_ = window_->Ok() && renderer_->Ok();
}

App::~App() noexcept {
  // `SDL_Quit()` must be the very last thing called
  // Systems (renderer, window) will be destroyed automatically by unique_ptr
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

  // Render here later

  renderer_->Present();
}