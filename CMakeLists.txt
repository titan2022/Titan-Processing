# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.20.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

option(ALGORITHMS "Whether to use Algorithms" ON)
option(CAMERA "Whether to use Camera" ON)
option(NETWORK "Whether to use NETWORK" OFF)

project ("Titan Processing")

set(CAMERA_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/Camera/include)
set(ALGORITHMS_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/Algorithms/include)
set(NETWORK_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/Network/include)

# Builds subdirectories
message("Building modules")
if(${ALGORITHMS})
	add_subdirectory("Algorithms")
endif()

if(${CAMERA})	
	add_subdirectory("Camera")
endif()

if(${NETWORK})
	add_subdirectory("Network")
endif()

message("Modules successfully built")

# Main executable
add_executable(visionDriver "visionDriver.cpp")

# Link subdirectories
message("Linking modules")
if(${ALGORITHMS})
	target_include_directories(visionDriver PRIVATE ${ALGORITHMS_INCLUDE})
	target_link_libraries(visionDriver algorithms)
	message("Algorithms module successfully linked")
endif()

if(${CAMERA})
	target_include_directories(visionDriver PRIVATE ${CAMERA_INCLUDE})
	target_link_libraries(visionDriver camera)
	message("Camera module successfully linked")
endif()

if(${NETWORK})
	target_include_directories(visionDriver PRIVATE ${NETWORK_INCLUDE})
	message("Network module successfully linked")
endif()

message("Modules successfully linked")