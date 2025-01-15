# Build service
add_executable(TitanProcessingService ${CMAKE_SOURCE_DIR}/src/TitanProcessingService.cpp)
target_include_directories(TitanProcessingService PUBLIC ${OpenCV_INCLUDE_DIRS} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(TitanProcessingService TitanProcessing ${OpenCV_LIBS})
install(TARGETS TitanProcessingService DESTINATION ${CMAKE_INSTALL_BINDIR})

# Build calibration script
add_executable(calibration ${CMAKE_SOURCE_DIR}/src/calibration.cpp ${SRC})
target_include_directories(calibration PUBLIC ${OpenCV_INCLUDE_DIRS} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(calibration ${OpenCV_LIBS} nlohmann_json::nlohmann_json)
set_target_properties(calibration PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
