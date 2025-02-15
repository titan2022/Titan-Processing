enable_testing()

find_package(GTest CONFIG REQUIRED)

# Requires Java 17
find_package(Java REQUIRED)
include(UseJava)
set(CMAKE_JAVA_COMPILE_FLAGS "-source" "17" "-target" "17")
file(GLOB_RECURSE JAVA_SRC ${CMAKE_CURRENT_SOURCE_DIR}/test/server/*.java)
add_jar(ServerTest ${JAVA_SRC} ENTRY_POINT ServerTest OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test)
get_target_property(_jarFile ServerTest JAR_FILE)
get_target_property(_classDir ServerTest CLASSDIR)

file(GLOB test_sources ${CMAKE_CURRENT_SOURCE_DIR}/test/client/*.cpp)
foreach(test_source ${test_sources})
    get_filename_component(test_source_name ${test_source} NAME_WE)
    add_executable(${test_source_name} ${test_source})
    target_include_directories(${test_source_name} PUBLIC ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(${test_source_name} PUBLIC gtest gtest_main TitanProcessing)
    set_target_properties(${test_source_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test)
    add_test(${test_source_name} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test/${test_source_name})
endforeach()

add_executable(test_client_exec ${CMAKE_CURRENT_SOURCE_DIR}/test/client/util/test_client_exec.cpp)
target_include_directories(test_client_exec PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(test_client_exec PUBLIC TitanProcessing)
set_target_properties(test_client_exec PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test/util)
