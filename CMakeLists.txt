cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_CXX_STANDARD 17)
project(TRBNetworking VERSION 1.0.0 LANGUAGES C CXX)

# Directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
include_directories(${CMAKE_SOURCE_DIR}/include)

# # RealSense linking
# if (NOT NO_REALSENSE)
#     include_directories(include ${Realsense_DIR}/include)
# endif()

# # OpenCV linking
# set(OpenCV_DIR /usr/lib/opencv)
# option(OpenCV_STATIC OFF)
# find_package(OpenCV 4)
# include_directories(${OpenCV_INCLUDE_DIRS})

# nlohmann/json linking
find_package(nlohmann_json CONFIG REQUIRED)

# Creating the library
file(GLOB SRC 
    # src/apriltag/*.cpp
    # src/helper/*.cpp
    src/networking/*.cpp
    # src/physics/*.cpp
)
# if (NOT NO_REALSENSE)
#     file(GLOB RS_SRC src/realsense/*.cpp)
#     add_library(TRBNeworking SHARED ${SRC} ${RS_SRC})
# else()
#     add_library(TRBNeworking ${SRC})
# endif()

add_library(TRBNeworking SHARED ${SRC})
install(TARGETS TRBNeworking DESTINATION TRBNeworking)

# Linking libraries
# target_link_libraries(TRBNeworking ${apriltag_LIBRARIES} ${OpenCV_LIBS} nlohmann_json::nlohmann_json)
target_link_libraries(TRBNeworking nlohmann_json::nlohmann_json)

# # Building examples
# if (BUILD_EXAMPLES)
#     file(GLOB_RECURSE EXAMPLE_SRC RELATIVE ${CMAKE_SOURCE_DIR}/example ${CMAKE_SOURCE_DIR}/example/*/*.cpp ${CMAKE_SOURCE_DIR}/example/*/*.c)
#     foreach(example ${EXAMPLE_SRC})
#         string(REPLACE ".cpp" "" examplename0 ${example})
#         string(REPLACE ".c" "" examplename1 ${examplename0})
#         string(REGEX REPLACE ".+\/" "" examplename ${examplename1})
#         add_executable(${examplename} ${CMAKE_SOURCE_DIR}/example/${example})
#         # target_link_libraries(${examplename} TRBNeworking ${apriltag_LIBRARIES} ${OpenCV_LIBS})
#         target_link_libraries(${examplename} TRBNeworking nlohmann_json::nlohmann_json)
#     endforeach(example ${EXAMPLE_SRC})
# endif()
