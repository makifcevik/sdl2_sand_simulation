cmake_minimum_required(VERSION 3.25)

# 1. Project Info
project(CppStarter VERSION 1.0.0 LANGUAGES CXX)

# 2. Enforce Standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 3. Find Dependencies (Vcpkg handles the paths)
find_package(fmt CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

# 4. Enable Testing
enable_testing()

# --- COMPONENT 1: THE MAIN APP ---
# We add main.cpp here.
add_executable(MyApp src/main.cpp)
target_link_libraries(MyApp PRIVATE fmt::fmt)

# --- COMPONENT 2: THE TESTS ---
# We discover all test files in the 'tests/' folder
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")

add_executable(UnitTests ${TEST_SOURCES})
# Link GTest main
target_link_libraries(UnitTests PRIVATE GTest::gtest_main fmt::fmt)

# Register the test with CTest (CMake's test runner)
include(GoogleTest)
gtest_discover_tests(UnitTests)