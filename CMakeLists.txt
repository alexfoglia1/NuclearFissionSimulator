cmake_minimum_required(VERSION 2.6)
project(FissionSim)

add_subdirectory(utils)

add_executable(
fission_simulator

yields.h
periodictable.h

${CMAKE_SOURCE_DIR}/utils/python_generator.h
${CMAKE_SOURCE_DIR}/utils/python_generator.cpp
${CMAKE_SOURCE_DIR}/utils/random_gen.h
${CMAKE_SOURCE_DIR}/utils/random_gen.cpp

fission_gen.h
fission_gen.cpp
fission_simulator.cpp
)
