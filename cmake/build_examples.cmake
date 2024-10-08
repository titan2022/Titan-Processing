file(GLOB_RECURSE EXAMPLE_SRC RELATIVE ${CMAKE_SOURCE_DIR}/example ${CMAKE_SOURCE_DIR}/example/*/*.cpp ${CMAKE_SOURCE_DIR}/example/*/*.c)
foreach(example ${EXAMPLE_SRC})
    string(REPLACE ".cpp" "" examplename0 ${example})
    string(REPLACE ".c" "" examplename1 ${examplename0})
    string(REGEX REPLACE ".+\/" "" examplename ${examplename1})
    add_executable(${examplename} ${CMAKE_SOURCE_DIR}/example/${example})
    target_include_directories(${examplename} PUBLIC ${OpenCV_INCLUDE_DIRS} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(${examplename} TitanProcessing ${OpenCV_LIBS})
endforeach(example ${EXAMPLE_SRC})