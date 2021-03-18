cmake_minimum_required(VERSION 3.17)
project(Simulater_Leraning)

set(CMAKE_CXX_STANDARD 14)

add_executable(test.cpp test.cpp)
add_executable(simbase.cpp simbase.cpp )
add_executable(Simulater_Leraning main.cpp workspace.h simbase.h)

target_link_libraries(test.cpp -lOpenGL32 -lfreeGLUT)
target_link_libraries(simbase.cpp -lOpenGL32 -lfreeGLUT)
target_link_libraries(Simulater_Leraning -lOpenGL32 -lfreeGLUT)
