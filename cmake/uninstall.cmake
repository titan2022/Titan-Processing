if(NOT TARGET uninstall)
  add_custom_target(uninstall COMMAND cd ${CMAKE_BINARY_DIR} && xargs rm < install_manifest.txt)
endif()