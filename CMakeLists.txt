cmake_minimum_required(VERSION 3.17)
project(Simulater_Leraning)

find_package(OpenMP REQUIRED)

set(CMAKE_C_FLAGS "\${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_C11_FLAGS "\${CMAKE_C11_FLAGS} ${OpenMP_C11_FLAGS}")

set(CMAKE_CXX_STANDARD 11)
add_compile_options(-Ofast)
add_executable(Simulater_Leraning main.cpp)
add_executable(glwindows test_glwindows.cpp)

target_link_libraries(Simulater_Leraning -lOpenGL32 -lfreeGLUT)
target_link_libraries(glwindows -lOpenGL32 -lfreeGLUT)
target_link_libraries(glwindows -lOpenGL32 -lfreeGLUT)