file(GLOB_RECURSE EXAMPLE_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/example ${CMAKE_CURRENT_SOURCE_DIR}/example/*/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/example/*/*.c)
foreach(example ${EXAMPLE_SRC})
    string(REPLACE ".cpp" "" examplename0 ${example})
    string(REPLACE ".c" "" examplename1 ${examplename0})
    string(REGEX REPLACE ".+\/" "" examplename ${examplename1})
    add_executable(${examplename} ${CMAKE_CURRENT_SOURCE_DIR}/example/${example})
    target_link_libraries(${examplename} PUBLIC TitanProcessing)
    target_include_directories(${examplename} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
endforeach(example ${EXAMPLE_SRC})
