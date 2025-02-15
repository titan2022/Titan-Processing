# Build service
add_executable(TitanProcessingService ${CMAKE_CURRENT_SOURCE_DIR}/src/TitanProcessingService.cpp)
target_include_directories(TitanProcessingService PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(TitanProcessingService PUBLIC TitanProcessing)
install(TARGETS TitanProcessingService DESTINATION ${CMAKE_INSTALL_BINDIR})

# Build calibration script
add_executable(calibration ${CMAKE_CURRENT_SOURCE_DIR}/src/calibration.cpp ${SRC})
target_include_directories(calibration PUBLIC ${OpenCV_INCLUDE_DIRS} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(calibration PUBLIC nlohmann_json::nlohmann_json ${OpenCV_LIBS} wpimath ntcore)
set_target_properties(calibration PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
