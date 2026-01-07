// MIT License

#ifndef SDL2_SAND_SIMULATION_APP_INPUT_H_
#define SDL2_SAND_SIMULATION_APP_INPUT_H_

#include <vector>

#include <SDL.h>

// Provides a wrapper class for inputs and events.
// Usage:
// Call `BeginFrame()` at the start of every loop iteration.
// Pass SDL events to `ProcessEvent()` during the polling phase.
class Input {
 public:
  Input();

  // Resets the single frame events.
  // Must be called at the very beginning of the frame (before event polling).
  void BeginFrame();

  // Updates the internal state based on the SDL event.
  void ProcessEvent(const SDL_Event& event);

  // Checks if a key pushed down THIS frame (ignores auto-repeat).
  bool IsKeyPressed(SDL_Scancode scan_code) const;
  bool IsKeyReleased(SDL_Scancode scan_code) const;

  // Cheks if a mouse button is HELD down.
  bool IsMouseButtonDown(uint8_t button) const;
  void GetMousePosition(int32_t* x_out, int32_t* y_out) const;

  bool QuitRequested() const;

 private:
  // Stores keys pressed this frame.
  std::vector<SDL_Scancode> pressed_keys_;
  // Stores keys released this frame.
  std::vector<SDL_Scancode> released_keys_;
  // Pointer to SDL's internal keyboard state array (managed by SDL).
  const uint8_t* keyboard_state_{nullptr};
  bool quit_requested_{false};
};

#endif  // SDL2_SAND_SIMULATION_APP_INPUT_H_
