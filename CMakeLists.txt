cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
project(TitanProcessing VERSION 2.0.0 LANGUAGES C CXX)

option(TEST "Build all tests." OFF)
option(BUILD_EXAMPLES "Build all examples." OFF)
option(NO_REALSENSE "Ignores RealSense components." OFF)

# Directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# OpenCV linking
set(OpenCV_DIR /usr/lib/opencv)
option(OpenCV_STATIC OFF)
find_package(OpenCV 4.8)

# nlohmann/json linking
find_package(nlohmann_json CONFIG REQUIRED)

# Shared library compatibility
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
include(GNUInstallDirs)

# Creating the library
include(${CMAKE_MODULE_PATH}/build_library.cmake)

# Creating service binary
include(${CMAKE_MODULE_PATH}/build_service.cmake)

# Building examples
if(BUILD_EXAMPLES)
    include(${CMAKE_MODULE_PATH}/build_examples.cmake)
endif()

# Testing
if(TEST)
    include(${CMAKE_MODULE_PATH}/build_tests.cmake)
endif()

# `make uninstall` script
include(${CMAKE_MODULE_PATH}/uninstall.cmake)
