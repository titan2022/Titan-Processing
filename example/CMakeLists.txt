cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_CXX_STANDARD 14)
project(examples VERSION 0.0.0 LANGUAGES C CXX)

message("eeeeeeeeee")

# Directories
include_directories(${CMAKE_SOURCE_DIR}/../include)

# RealSense linking
#include_directories(include ${realsense_INCLUDE_DIR})

# OpenCV linking
set(OpenCV_DIR "/usr/lib/opencv")
option(OpenCV_STATIC OFF)
find_package(OpenCV 4 REQUIRED core imgproc highgui)
include_directories(${OpenCV_INCLUDE_DIRS})

# Titan Processing linking
add_library(TitanProcessing SHARED IMPORTED)

# Creating the executables
file(GLOB EXAMPLE_SRC RELATIVE ${CMAKE_SOURCE_DIR}/apriltag ${CMAKE_SOURCE_DIR}/apriltag/*.cpp)
foreach(example ${EXAMPLE_SRC})
    message("aa")
    string(REPLACE ".cpp" "" examplename ${example})
    add_executable(${examplename} apriltag/${example})
    target_link_libraries(${examplename} TitanProcessing)
endforeach(example ${EXAMPLE_SRC})