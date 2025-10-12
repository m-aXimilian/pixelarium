set(SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/custom_1.cpp)

set(CUSTOM_1_NAME "${PROJECT_NAME}_custom1_app")

add_executable(${CUSTOM_1_NAME} ${SRC})

target_link_libraries(${CUSTOM_1_NAME}
  PRIVATE pixelariumimagelib
  PRIVATE pixelariumrenderlib
  PRIVATE pixelariumutilslib
  PRIVATE pixelariumresourcelib
  PRIVATE pixelariumapplicationlib)

target_include_directories(${CUSTOM_1_NAME}
  PRIVATE ${PROJECT_SOURCE_DIR}/src
  PRIVATE ${PROJECT_SOURCE_DIR}/lib
  PRIVATE ${PROJECT_SOURCE_DIR}/lib/imaging
  PRIVATE ${PROJECT_SOURCE_DIR}/lib/app
  PRIVATE ${spdlog_DIR}/include
  PRIVATE ${LIBCZI_INCLUDE_DIR}
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
