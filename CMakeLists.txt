cmake_minimum_required(VERSION 3.25)

# 1. Project Info
project(SandSimulation VERSION 1.0.0 LANGUAGES CXX)

# 2. Enforce Standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 3. Find Dependencies (Vcpkg handles the paths)
find_package(fmt CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

# 4. Enable Testing
enable_testing()

# Add Subdirectories
add_subdirectory(app)
add_subdirectory(core)
add_subdirectory(tests)