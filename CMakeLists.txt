cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_CXX_STANDARD 17)
project(TitanProcessing VERSION 0.0.0 LANGUAGES C CXX)

# Directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
include_directories(${CMAKE_SOURCE_DIR}/include)

# RealSense linking
if (NOT NO_REALSENSE)
    include_directories(include ${Realsense_DIR}/include)
endif()

# OpenCV linking
set(OpenCV_DIR /usr/lib/opencv)
option(OpenCV_STATIC OFF)
find_package(OpenCV 4)
include_directories(${OpenCV_INCLUDE_DIRS})

# Apriltag linking
# find_package(PkgConfig)
# pkg_search_module(apriltag REQUIRED apriltag)
# set(apriltag_INCLUDE_DIRS "${apriltag_INCLUDE_DIRS}/apriltag")
# link_directories(${apriltag_LIBDIR})
# include_directories(include ${apriltag_INCLUDE_DIRS})

# nlohmann/json linking
find_package(nlohmann_json CONFIG REQUIRED)

# Creating the library
file(GLOB SRC 
    src/apriltag/*.cpp
    src/helper/*.cpp
    src/networking/*.cpp
    src/physics/*.cpp
)
if (NOT NO_REALSENSE)
    file(GLOB RS_SRC src/realsense/*.cpp)
    add_library(TitanProcessing SHARED ${SRC} ${RS_SRC})
else()
    add_library(TitanProcessing ${SRC})
endif()
install(TARGETS TitanProcessing DESTINATION TitanProcessing)

# Linking libraries
target_link_libraries(TitanProcessing ${apriltag_LIBRARIES} ${OpenCV_LIBS} nlohmann_json::nlohmann_json)

# Building examples
if (BUILD_EXAMPLES)
    file(GLOB_RECURSE EXAMPLE_SRC RELATIVE ${CMAKE_SOURCE_DIR}/example ${CMAKE_SOURCE_DIR}/example/*/*.cpp)
    foreach(example ${EXAMPLE_SRC})
        string(REPLACE ".cpp" "" examplename0 ${example})
        string(REGEX REPLACE ".+\/" "" examplename ${examplename0})
        add_executable(${examplename} ${CMAKE_SOURCE_DIR}/example/${example})
        target_link_libraries(${examplename} TitanProcessing ${apriltag_LIBRARIES} ${OpenCV_LIBS})
    endforeach(example ${EXAMPLE_SRC})
endif()
