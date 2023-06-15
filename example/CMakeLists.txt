cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_CXX_STANDARD 14)
project(examples VERSION 0.0.0 LANGUAGES C CXX)

# Directories

# RealSense linking
include_directories(include ${realsense_INCLUDE_DIR})

# OpenCV linking
set(OpenCV_DIR "/usr/lib/opencv")
option(OpenCV_STATIC OFF)
find_package(OpenCV 4 REQUIRED core imgproc highgui)
include_directories(${OpenCV_INCLUDE_DIRS})

# Titan Processing linking
add_library(TitanProcessing SHARED IMPORTED)
include_directories(include ${realsense_INCLUDE_DIR})

# Creating the library
file(GLOB_RECURSE SRC ${EXAMPLE})
add_library(${PROJECT_NAME} SHARED ${SRC})
install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_NAME})

#target_link_libraries(realsense realsense2)
#target_link_libraries(realsense ${OpenCV_LIBS})