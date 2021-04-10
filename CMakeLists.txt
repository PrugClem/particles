# CMakeList.txt : CMake project for particles, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("particles")

# Add source to this project's executable.
add_executable (particles 
    "main.cpp" "app.cpp" "app.hpp" "particles.hpp"  # c++ source files for the program        
)

# copy shaders (copy from $src/shaders/* to $bin/shaders/*)
add_custom_command(TARGET particles POST_BUILD
COMMAND "robocopy" ARGS "\"${CMAKE_CURRENT_SOURCE_DIR}/assets\"" "\"${CMAKE_BINARY_DIR}/assets\"" "\"*.*\"" "/e" "/mir" "/njh" "/njs" "/ndl"
)

# OpenGL stuff thanks to windows not convenient
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/glew-2.1.0/include"
                    "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.3/include"
                    "${CMAKE_CURRENT_SOURCE_DIR}/lib/glm")

# OpenMP
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(particles PUBLIC OpenMP::OpenMP_CXX)
endif()

# R-Michi's GLshader library
add_subdirectory(GLshader)
target_link_libraries(particles PUBLIC gl_shader)

# OpenGL stuff, thanks to windows not convenient
target_link_libraries(particles PUBLIC
"-l${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.3/lib/glfw3"
"-l${CMAKE_CURRENT_SOURCE_DIR}/lib/glew-2.1.0/lib/Release/x64/glew32s"
"-l${CMAKE_CURRENT_SOURCE_DIR}/lib/glm/glm/lib/glm_static"
"-lopengl32")
