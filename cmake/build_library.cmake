file(GLOB SRC 
    src/apriltag/*.cpp
    src/util/*.cpp
    src/networking/*.cpp
    src/physics/*.cpp
)
file(GLOB HEADERS 
    include/apriltag/*.hpp
    include/util/*.hpp
    include/networking/*.hpp
    include/physics/*.hpp
)
if (NOT NO_REALSENSE)
    file(GLOB RS_SRC src/realsense/*.cpp)
    add_library(TitanProcessing SHARED ${SRC} ${RS_SRC})
    target_include_directories(TitanProcessing PRIVATE ${Realsense_DIR}/include)
else()
    add_library(TitanProcessing SHARED ${SRC})
endif()
target_include_directories(TitanProcessing PUBLIC ${OpenCV_INCLUDE_DIRS} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(TitanProcessing ${OpenCV_LIBS} nlohmann_json::nlohmann_json)
set_target_properties(TitanProcessing PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TitanProcessing PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(TitanProcessing PROPERTIES PUBLIC_HEADER "${HEADERS}")

# INCLUDES DIRECTORY ${CMAKE_SOURCE_DIR}/include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/TitanProcessing FILES_MATCHING PATTERN "*.hpp"
#         PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/TitanProcessing

install(TARGETS TitanProcessing EXPORT
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/TitanProcessing
)

# foreach(file ${HEADERS})
#     # get_filename_component(subdir ${file} DIRECTORY)
#     cmake_path(GET file PARENT_PATH fullSubdir)
#     cmake_path(GET fullSubdir STEM subdir)
#     install(FILES ${file} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/TitanProcessing/${subdir})
# endforeach()