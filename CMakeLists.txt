cmake_minimum_required(VERSION 3.0)
project(lab2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package(OpenGL REQUIRED)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_subdirectory(external)

include_directories(
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glad-opengl-3.3/include/
	external/
	lab2/
)

add_executable(lab2
	lab2/lab2.cpp
	lab2/render/shader.cpp
	lab2/render/texture.cpp
)
target_link_libraries(lab2
	${OPENGL_LIBRARY}
	glfw
	glad
)
