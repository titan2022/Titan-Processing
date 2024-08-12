cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_CXX_STANDARD 20)
project(TitanProcessing VERSION 1.0.0 LANGUAGES C CXX)

option(TEST "Build all tests." OFF)

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

# nlohmann/json linking
find_package(nlohmann_json CONFIG REQUIRED)

# Creating the library
file(GLOB SRC 
    src/apriltag/*.cpp
    src/util/*.cpp
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
    file(GLOB_RECURSE EXAMPLE_SRC RELATIVE ${CMAKE_SOURCE_DIR}/example ${CMAKE_SOURCE_DIR}/example/*/*.cpp ${CMAKE_SOURCE_DIR}/example/*/*.c)
    foreach(example ${EXAMPLE_SRC})
        string(REPLACE ".cpp" "" examplename0 ${example})
        string(REPLACE ".c" "" examplename1 ${examplename0})
        string(REGEX REPLACE ".+\/" "" examplename ${examplename1})
        add_executable(${examplename} ${CMAKE_SOURCE_DIR}/example/${example})
        target_link_libraries(${examplename} TitanProcessing ${apriltag_LIBRARIES} ${OpenCV_LIBS})
    endforeach(example ${EXAMPLE_SRC})
endif()

if (TEST)
    enable_testing()

    find_package(GTest CONFIG REQUIRED)

    # Server testing
    # Requires Java 17
    find_package(Java REQUIRED)
    include(UseJava)
    set(CMAKE_JAVA_COMPILE_FLAGS "-source" "17" "-target" "17")
    file(GLOB_RECURSE JAVA_SRC ${CMAKE_SOURCE_DIR}/test/server/*.java)
    add_jar(ServerTest ${JAVA_SRC} ENTRY_POINT ServerTest OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test)
    get_target_property(_jarFile ServerTest JAR_FILE)
    get_target_property(_classDir ServerTest CLASSDIR)

    # message(STATUS "Jar file ${_jarFile}")
    # message(STATUS "Class compiled to ${_classDir}")
    # add_test(NAME TestServer COMMAND ${Java_JAVA_EXECUTABLE} -cp ${_jarFile} ServerTest WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

    file(GLOB_RECURSE test_sources ${CMAKE_SOURCE_DIR}/test/*.cpp)
    foreach(test_source ${test_sources})
        get_filename_component(test_source_name ${test_source} NAME_WE)
        add_executable(${test_source_name} ${test_source})
        set_target_properties(${test_source_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test)
        target_link_libraries(${test_source_name} gtest gtest_main TitanProcessing ${apriltag_LIBRARIES} ${OpenCV_LIBS})
        add_test(${test_source_name} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test/${test_source_name})
    endforeach()
endif()