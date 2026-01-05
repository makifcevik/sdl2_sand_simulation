add_executable(Simulation "main.cc")
target_link_libraries(Simulation PRIVATE
	core_lib
	fmt::fmt
	SDL2::SDL2
	SDL2::SDL2main
)
