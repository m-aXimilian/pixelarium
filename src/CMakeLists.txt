message(STATUS "Configuring Resources")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/app_resources_local.h.in
  ${CMAKE_BINARY_DIR}/app_resources_local.h @ONLY)
