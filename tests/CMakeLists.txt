# Discover all test files in the folder
file(GLOB_RECURSE TEST_SOURCES "./*.cc")

add_executable(UnitTests ${TEST_SOURCES})
# Link GTest main
target_link_libraries(UnitTests PRIVATE GTest::gtest_main fmt::fmt)

# Register the test with CTest (CMake's test runner)
include(GoogleTest)
gtest_discover_tests(UnitTests)