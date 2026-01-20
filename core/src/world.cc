// MIT License

#include "world.h"

World::World(int32_t width, int32_t height) : width_(width), height_(height) {
  cells_.resize(width * height, CellType::kEmpty);
}

void World::Update(uint32_t frame_count) {
  // Iterate bottom to top
  // Iterate left to right
  for (int32_t y = height_ - 1; y >= 0; --y) {

    // Alternating x direction
    bool flow_right = (frame_count & 1) == 0;

    int32_t start_x = flow_right ? 0 : width_ - 1;
    int32_t end_x = flow_right ? width_ : -1;  // Loop ends when x equals this
    int32_t step_x = flow_right ? 1 : -1;

    for (int32_t x = start_x; x != end_x; x += step_x) {

      // Calculate the index of the current cell
      int32_t i = y * width_ + x;

      // If (current) cell empty, skip it.
      if (cells_[i] == CellType::kEmpty)
        continue;

      if (cells_[i] == CellType::kSand) {

        // If it is floor, skip it.
        if ((y + 1) >= height_)
          continue;

        // Calculate the index of the cell below
        int32_t below_i = (y + 1) * width_ + x;

        // Rule 1: Fall straight down if empty
        if (cells_[below_i] == CellType::kEmpty) {
          cells_[below_i] = CellType::kSand;
          cells_[i] = CellType::kEmpty;
        }
        // Rule 2: Slide down-left or down-right (Simple friction)
        else {
          // "Free" Randomness: Use parity of coordinates + frame count.
          // This creates a checkerboard pattern that flips every frame.
          bool try_left_first = (x + y + frame_count) & 1;

          // Determine Primary and Secondary offsets based on that boolean.
          // first direction x, second direction x
          int32_t first_dx = try_left_first ? -1 : 1;
          int32_t second_dx = try_left_first ? 1 : -1;

          // Try primary direction.
          int32_t below_primary = below_i + first_dx;
          if (x + first_dx >= 0 && x + first_dx < width_ &&
              cells_[below_primary] == CellType::kEmpty) {
            cells_[below_primary] = CellType::kSand;
            cells_[i] = CellType::kEmpty;
          }
          // Try secondary direction.
          else {
            int32_t below_secondary = below_i + second_dx;
            if (x + second_dx >= 0 && x + second_dx < width_ &&
                cells_[below_secondary] == CellType::kEmpty) {
              cells_[below_secondary] = CellType::kSand;
              cells_[i] = CellType::kEmpty;
            }
          }
        }  // End of rules
      }  // End of CellType::kSand if
    }  // End of inner for loop
  }  // End of outer for loop
}

// Internally checks if coordinates are valid
// Updates only if the type provided differs from the cell type at that coords.
// (Reqired to safely update the sand_count_)
void World::SetCell(int32_t x, int32_t y, CellType type) {
  if (IsValid(x, y)) {
    if (type != cells_[width_ * y + x]) {

      if (type == CellType::kSand)
        sand_count_++;
      else
        sand_count_--;

      cells_[width_ * y + x] = type;
    }
  }
}

World::CellType World::GetCell(int32_t x, int32_t y) const {
  if (IsValid(x, y)) {
    return cells_[width_ * y + x];
  }
  // Return empty cell if invalid
  return CellType::kEmpty;
}

bool World::IsValid(int32_t x, int32_t y) const {
  return x >= 0 && y >= 0 && x < width_ && y < height_;
}