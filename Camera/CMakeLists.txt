

message("Building Camera module")

set(REALSENSE_INCLUDE $ENV{INTEL_REALSENSE}/include)

find_library(REALSENSE_LIBRARY realsense2 PATHS $ENV{INTEL_REALSENSE}/lib/x64)
find_package(OpenCV REQUIRED)

file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_library(camera STATIC ${SOURCES})

target_include_directories(camera PUBLIC ${REALSENSE_INCLUDE})
target_include_directories(camera PRIVATE ${OpenCV_INCLUDE_DIRS})
target_include_directories(camera PRIVATE ${CAMERA_INCLUDE})
target_include_directories(camera PRIVATE ${ALGORITHMS_INCLUDE})

target_link_libraries(camera ${REALSENSE_LIBRARY})
target_link_libraries(camera ${OpenCV_LIBS})

message("Camera module successfully built")