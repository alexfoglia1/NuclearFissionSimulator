cmake_minimum_required(VERSION 2.6)
project(fission_simulator)

add_executable(
fission_simulator

${CMAKE_SOURCE_DIR}/src/models/yields.h
${CMAKE_SOURCE_DIR}/src/models/periodictable.h
${CMAKE_SOURCE_DIR}/src/models/fission_gen.h
${CMAKE_SOURCE_DIR}/src/models/periodictable.cpp
${CMAKE_SOURCE_DIR}/src/models/fission_gen.cpp

${CMAKE_SOURCE_DIR}/src/utils/python_generator.h
${CMAKE_SOURCE_DIR}/src/utils/python_generator.cpp
${CMAKE_SOURCE_DIR}/src/utils/random_gen.h
${CMAKE_SOURCE_DIR}/src/utils/random_gen.cpp

${CMAKE_SOURCE_DIR}/src/main/fission_simulator.cpp
)

target_include_directories(fission_simulator PRIVATE ${CMAKE_SOURCE_DIR}/src/models)
target_include_directories(fission_simulator PRIVATE ${CMAKE_SOURCE_DIR}/src/main)
target_include_directories(fission_simulator PRIVATE ${CMAKE_SOURCE_DIR}/src/utils)
