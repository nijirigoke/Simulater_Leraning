cmake_minimum_required(VERSION 3.17)
project(Simulater_Leraning)

set(CMAKE_CXX_STANDARD 14)


add_executable(Simulater_Leraning main.cpp workspace.h simbase.h)
add_executable(pachinko.cpp pachinko.cpp)

target_link_libraries(Simulater_Leraning -lOpenGL32 -lfreeGLUT)
