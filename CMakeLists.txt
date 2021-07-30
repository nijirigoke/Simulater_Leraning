cmake_minimum_required(VERSION 3.17)
project(Simulater_Leraning)

set(CMAKE_CXX_STANDARD 14)


add_executable(Simulater_Leraning main.cpp workspace.h simbase.h)
add_executable(test test.cpp)
add_executable(time testtime.cpp)
add_executable(test2 test2.cpp)

target_link_libraries(Simulater_Leraning -lOpenGL32 -lfreeGLUT)
target_link_libraries(test -lOpenGL32 -lfreeGLUT)
target_link_libraries(test2 -lOpenGL32 -lfreeGLUT)
target_link_libraries(time -lOpenGL32 -lfreeGLUT)

