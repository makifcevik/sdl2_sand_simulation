add_executable(Simulation "main.cc" 
"window.cc" "renderer.cc" "input.cc" "app.cc" "texture.cc")
target_link_libraries(Simulation PRIVATE
	core_lib
	fmt::fmt
	SDL2::SDL2
	SDL2::SDL2main
)
