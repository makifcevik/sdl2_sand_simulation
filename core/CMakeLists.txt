add_library(core_lib STATIC
	src/world.cc
)

target_include_directories(core_lib PUBLIC include)