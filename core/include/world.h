// MIT License

#ifndef SDL2_SAND_SIMULATION_CORE_SRC_WORLD_H_
#define SDL2_SAND_SIMULATION_CORE_SRC_WORLD_H_

#include <cstdint>

#include <vector>

// Defines the main world.
// Simulation is calculated in this class.
// Set the simulation width and height in the constructor.
// Use `GetCells()` to retrieve all the underlying cells.
// Call the `Update()` method to run the simulation.
class World {
 public:
  enum class CellType : uint8_t { kEmpty = 0, kSand = 1 };

  // Set the simulation width and height.
  World(int32_t width, int32_t height);

  // Handles its own internal logic.
  // Pass the frame_count (required for randomness)
  void Update(uint32_t frame_count);

  // Internally checks if coordinates are valid
  // Updates only if the type provided differs from the cell type at that coords.
  // (Reqired to safely update the sand_count_)
  void SetCell(int32_t x, int32_t y, CellType type);

  // Internally checks if coordinates are valid
  CellType GetCell(int32_t x, int32_t y) const;

  int32_t GetWidth() const { return width_; };
  int32_t GetHeight() const { return height_; };
  uint64_t GetSandCount() const { return sand_count_; }

  // You can use it to access the underlying cells (can be fed to a graphics API).
  const std::vector<CellType>& GetCells() const { return cells_; };

 private:
  std::vector<CellType> cells_;
  uint64_t sand_count_{0};
  int32_t width_;
  int32_t height_;

  bool IsValid(int32_t x, int32_t y) const;
};

#endif  // SDL2_SAND_SIMULATION_CORE_SRC_WORLD_H_
