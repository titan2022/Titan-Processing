
message("Building Algorithms module")

find_package(OpenCV REQUIRED)

file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_library(algorithms STATIC ${SOURCES} )

target_include_directories(algorithms PRIVATE ${OpenCV_INCLUDE_DIRS})
target_include_directories(algorithms PRIVATE ${ALGORITHMS_INCLUDE})

target_link_libraries(algorithms ${OpenCV_LIBS})

message("Algorithms module successfully built")