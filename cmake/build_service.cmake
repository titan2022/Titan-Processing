add_executable(TitanProcessingService ${CMAKE_SOURCE_DIR}/src/TitanProcessingService.cpp)
target_include_directories(TitanProcessingService PUBLIC ${OpenCV_INCLUDE_DIRS} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(TitanProcessingService TitanProcessing ${OpenCV_LIBS})
install(TARGETS TitanProcessingService DESTINATION ${CMAKE_INSTALL_BINDIR})