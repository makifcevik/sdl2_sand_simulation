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

  if (texture_->Ok()) {
    // Construct the world
    world_ =
        std::make_unique<World>(texture_->GetWidth(), texture_->GetHeight());
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

  // Input and world are independent, but good to clean up.
  input_.reset();
  world_.reset();

  // `SDL_Quit()` must be the very last thing called
  SDL_Quit();
}

int App::Run() {
  if (!is_running_)
    return 1;  // Error code

  uint64_t last_time = SDL_GetTicks64();
  double dt{0.0};
  float fps_timer = 0.0f;
  uint32_t frame_count = 0;

  while (is_running_) {
    uint64_t current_time = SDL_GetTicks64();
    uint64_t frame_time = current_time - last_time;
    last_time = current_time;
    dt = static_cast<double>(frame_time) / 1000.0;

    frame_count++;
    fps_timer += dt;

    if (fps_timer > 0.1) {
      std::string title =
          "Brush Size: " + std::to_string(brush_size_) +
          "   Sand Count: " + std::to_string(world_->GetSandCount()) +
          "   FPS: " +
          std::to_string(static_cast<int32_t>(frame_count * (1 / fps_timer)));
      SDL_SetWindowTitle(window_->Get(), title.c_str());

      frame_count = 0;
      fps_timer = 0.0f;
    }

    PollEvents();
    Update(frame_count);
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

void App::Update(uint32_t frame_count) {

  // --- Quit logic
  if (input_->QuitRequested()) {
    is_running_ = false;
  }
  if (input_->IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
    is_running_ = false;
  }

  // Checks the mouse scroll wheel and updates the brush size.
  ModifyBrushSize();

  // --- Spawn sand with left mouse button
  if (input_->IsMouseButtonDown(SDL_BUTTON_LEFT)) {
    SpawnSand(frame_count);
  }
  // --- Destroy sand with right mouse button
  if (input_->IsMouseButtonDown(SDL_BUTTON_RIGHT)) {
    DestroySand(frame_count);
  }

  // --- Run the simulation step
  world_->Update(frame_count);
}

void App::Render() {
  renderer_->Clear();

  // Convert world cells to pixel colors.
  const auto& cells = world_->GetCells();

  // Iterate through every cell.
  for (size_t i = 0; i < cells.size(); ++i) {
    uint32_t color = 0x00'00'00'FF;  // Default color (empty cell).
    if (cells[i] == World::CellType::kSand) {
      color = 0xB89B35FF;  // Sand Color
    }
    pixel_buffer_[i] = color;
  }

  // Upload and draw.
  texture_->Update(pixel_buffer_);
  renderer_->RenderFrame(texture_->Get());
  renderer_->Present();
}

void App::SpawnSand(uint32_t frame_count) {
  int32_t mouse_x, mouse_y;
  input_->GetMousePosition(&mouse_x, &mouse_y);

  // CRITICAL: Convert screen coordinates to world coordinates
  // Texture (world) size might not be equal to the window size
  float scale_x =
      static_cast<float>(texture_->GetWidth()) / window_->GetWidth();
  float scale_y =
      static_cast<float>(texture_->GetHeight()) / window_->GetHeight();

  int32_t world_x = static_cast<int32_t>(mouse_x * scale_x);
  int32_t world_y = static_cast<int32_t>(mouse_y * scale_y);

  Draw(World::CellType::kSand, world_x, world_y);
}

void App::DestroySand(uint32_t frame_count) {
  int32_t mouse_x, mouse_y;
  input_->GetMousePosition(&mouse_x, &mouse_y);

  // CRITICAL: Convert screen coordinates to world coordinates
  // Texture (world) size might not be equal to the window size
  float scale_x =
      static_cast<float>(texture_->GetWidth()) / window_->GetWidth();
  float scale_y =
      static_cast<float>(texture_->GetHeight()) / window_->GetHeight();

  int32_t world_x = static_cast<int32_t>(mouse_x * scale_x);
  int32_t world_y = static_cast<int32_t>(mouse_y * scale_y);

  Draw(World::CellType::kEmpty, world_x, world_y);
}

void App::ModifyBrushSize() {
  brush_size_ += input_->GetScrollDelta();
  if (brush_size_ < MIN_BRUSH_SIZE)
    brush_size_ = MIN_BRUSH_SIZE;
  if (brush_size_ > MAX_BRUSH_SIZE)
    brush_size_ = MAX_BRUSH_SIZE;
}

void App::Draw(World::CellType type, int32_t world_x, int32_t world_y) {
  // Simple brush
  for (int y = -brush_size_; y <= brush_size_; ++y) {
    for (int x = -brush_size_; x <= brush_size_; ++x) {
      // Circle check
      if (x * x + y * y < brush_size_ * brush_size_) {
        world_->SetCell(world_x + x, world_y + y, type);
      }
    }  // End of the inner for loop
  }  // End of the outer for loop
}