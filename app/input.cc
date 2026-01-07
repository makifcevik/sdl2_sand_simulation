// MIT License

#include "input.h"

Input::Input() {
  // Pre-allocate memory to avoid re-allocation during the first few frames.
  // 8 is an arbitrary small number that should cover most simultaneous inputs.
  pressed_keys_.reserve(8);
  released_keys_.reserve(8);
}

void Input::BeginFrame() {
  // Clear from last frame
  pressed_keys_.clear();
  released_keys_.clear();

  // Initialize the keyboard state once (SDL handles the rest).
  if (!keyboard_state_) {
    keyboard_state_ = SDL_GetKeyboardState(nullptr);
  }
}

void Input::ProcessEvent(const SDL_Event& event) {

  switch (event.type) {

    case SDL_QUIT:
      quit_requested_ = true;
      break;

    case SDL_KEYDOWN:
      // Ignore auto-repeat.
      if (event.key.repeat == 0) {
        pressed_keys_.push_back(event.key.keysym.scancode);
      }
      break;

    case SDL_KEYUP:
      released_keys_.push_back(event.key.keysym.scancode);
      break;

  }  // End of switch statement
}

bool Input::IsKeyPressed(SDL_Scancode scan_code) const {
  for (const auto pressed_key : pressed_keys_) {
    if (pressed_key == scan_code) {
      return true;
    }
  }  // End of for loop
  return false;
}

bool Input::IsKeyReleased(SDL_Scancode scan_code) const {
  for (const auto released_key : released_keys_) {
    if (released_key == scan_code) {
      return true;
    }
  }  // End of for loop
  return false;
}

bool Input::IsMouseButtonDown(uint8_t button) const {
  const uint32_t mask = SDL_GetMouseState(nullptr, nullptr);
  return (mask & SDL_BUTTON(button));
}
void Input::GetMousePosition(int32_t* x_out, int32_t* y_out) const {
  // Queries the window manager for the latest cursor position.
  SDL_GetMouseState(x_out, y_out);
}

bool Input::QuitRequested() const {
  return quit_requested_;
}