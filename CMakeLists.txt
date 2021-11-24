cmake_minimum_required(VERSION 3.17)
project(Simulater_Leraning)

set(CMAKE_CXX_STANDARD 11)
add_compile_options(-Ofast)
add_executable(Simulater_Leraning main.cpp)
add_executable(glwindows test_glwindows.cpp)

target_link_libraries(Simulater_Leraning -lOpenGL32 -lfreeGLUT)
target_link_libraries(glwindows -lOpenGL32 -lfreeGLUT)
target_link_libraries(glwindows -lOpenGL32 -lfreeGLUT)


