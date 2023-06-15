cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_CXX_STANDARD 14)
project(TitanProcessing VERSION 0.0.0 LANGUAGES C CXX)

# Directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include_directories(${CMAKE_SOURCE_DIR}/include)

# RealSense linking
if (NOT NO_REALSENSE)
    include_directories(include ${Realsense_DIR}/include)
endif()

# OpenCV linking
set(OpenCV_DIR "/usr/lib/opencv")
option(OpenCV_STATIC OFF)
find_package(OpenCV 4 REQUIRED core imgproc highgui videoio)
include_directories(${OpenCV_INCLUDE_DIRS})

# Apriltag linking
set(Apriltag_DIR "/usr/local/include/apriltag")
set(Apriltag_LIBS "${Apriltag_DIR}\\build\\Debug")
include_directories(${Apriltag_DIR})

# Creating the library
file(GLOB SRC 
    src/apriltag/*.cpp
    src/helper/*.cpp
    src/networking/*.cpp
)
if (NOT NO_REALSENSE)
    file(GLOB RS_SRC src/realsense/*.cpp)
    add_library(TitanProcessingLib SHARED ${SRC} ${RS_SRC})
else()
    add_library(TitanProcessingLib SHARED ${SRC})
endif()
install(TARGETS TitanProcessingLib DESTINATION TitanProcessingLib)

# Linking libraries
target_link_libraries(TitanProcessingLib ${OpenCV_LIBS})
target_link_libraries(TitanProcessingLib "${Apriltag_LIBS}\\apriltagd.dll")