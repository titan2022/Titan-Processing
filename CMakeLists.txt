cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_CXX_STANDARD 14)
project(TitanProcessing VERSION 0.0.0 LANGUAGES C CXX)

# Directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include_directories(${CMAKE_SOURCE_DIR}/include)

# RealSense linking
include_directories(include ${realsense_INCLUDE_DIR})

# OpenCV linking
set(OpenCV_DIR "/usr/lib/opencv")
option(OpenCV_STATIC OFF)
find_package(OpenCV 4 REQUIRED core imgproc highgui)
include_directories(${OpenCV_INCLUDE_DIRS})

# Creating the library
file(GLOB_RECURSE SRC src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SRC})
install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_NAME})

#target_link_libraries(realsense realsense2)
#target_link_libraries(realsense ${OpenCV_LIBS})