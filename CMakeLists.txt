cmake_minimum_required(VERSION 2.6)
project(cpp_SAT_phase_transition CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-O2 -std=c++17")

add_subdirectory(./Rustic_SAT_Solver)

add_executable(SAT_phase_transition main.cpp)
target_link_libraries(SAT_phase_transition rustic_sat)

