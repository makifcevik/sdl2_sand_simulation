// MIT License

#ifndef SDL2_SAND_SIMULATION_APP_APP_H_
#define SDL2_SAND_SIMULATION_APP_APP_H_

#include <memory>

#include "input.h"
#include "renderer.h"
#include "texture.h"
#include "window.h"
#include "world.h"

// This class acts like the "Director"
// It owns `Window`, `Renderer`, and `Input` systems and
// manages the Game Loop
class App {
 public:
  struct Config {
    Window::Config window_config;
    Renderer::Config renderer_config;
  };

  // Constructor with default configuration values
  App() : App(Config{}) {}

  explicit App(const Config&);
  ~App() noexcept;

  // Disallow copies
  App(const App&) = delete;
  App& operator=(const App&) = delete;

  // Disallow moves (App is the ROOT object, moving it risks killing SDL)
  App(App&&) = delete;
  App& operator=(App&&) = delete;

  // The main entry point. Returns the exit code (0 for success).
  int Run();

 private:
  // The 3 phases of the game loop
  void PollEvents();
  void Update(uint32_t frame_count);
  void Render();

  void SpawnSand(uint32_t frame_count);
  void DestroySand(uint32_t frame_count);
  void ModifyBrushSize();

  // Uses brush to draw cells.
  void Draw(World::CellType type, int32_t x, int32_t y);

  std::vector<uint32_t> pixel_buffer_;

  // Systems (order matters! Window must be created before the Renderer).
  std::unique_ptr<Window> window_;
  std::unique_ptr<Renderer> renderer_;
  std::unique_ptr<Texture> texture_;
  std::unique_ptr<Input> input_;
  std::unique_ptr<World> world_;
  
  const int32_t MIN_BRUSH_SIZE = 2;
  const int32_t MAX_BRUSH_SIZE = 256;
  int32_t brush_size_{32};

  bool is_running_{false};
};

#endif  // SDL2_SAND_SIMULATION_APP_APP_H_
