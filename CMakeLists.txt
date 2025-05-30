cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_CXX_STANDARD 20)
project(TitanProcessing VERSION 2.0.0 LANGUAGES C CXX)

# Generate compile_commands.json by default
if(NOT CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_EXPORT_COMPILE_COMMANDS "YES" CACHE STRING "" FORCE)
endif()

option(TEST "Build all tests." OFF)
option(BUILD_EXAMPLES "Build all examples." OFF)
option(NO_REALSENSE "Ignores RealSense components." OFF)

# Directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# OpenCV linking
set(OpenCV_DIR /usr/lib/opencv)
option(OpenCV_STATIC OFF)
find_package(OpenCV 4.8)

# nlohmann/json linking
find_package(nlohmann_json CONFIG REQUIRED)

# Add wpimath and ntcore
find_package(wpilib REQUIRED)

# Shared library compatibility
SET(CMAKE_SKIP_BUILD_RPATH FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
include(GNUInstallDirs)

# Creating the library
include(${CMAKE_MODULE_PATH}/build_library.cmake)

# Creating service binary
include(${CMAKE_MODULE_PATH}/build_service.cmake)

# Building examples
if (BUILD_EXAMPLES)
    include(${CMAKE_MODULE_PATH}/build_examples.cmake)
endif()

# Testing
if (TEST)
    include(${CMAKE_MODULE_PATH}/build_tests.cmake)
endif()

# `make uninstall` script
include(${CMAKE_MODULE_PATH}/uninstall.cmake)
