set(SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

set(CUSTOM_0_NAME "${PROJECT_NAME}_custom_app")

add_executable(${CUSTOM_0_NAME} ${SRC})

target_link_libraries(${CUSTOM_0_NAME}
  PRIVATE pixelariumimagelib
  PRIVATE pixelariumrenderlib
  PRIVATE pixelariumutilslib
  PRIVATE pixelariumresourcelib
  PRIVATE pixelariumapplicationlib)

target_include_directories(${CUSTOM_0_NAME}
  PRIVATE ${PROJECT_SOURCE_DIR}/src
  PRIVATE ${PROJECT_SOURCE_DIR}/lib
  PRIVATE ${PROJECT_SOURCE_DIR}/lib/imaging
  PRIVATE ${PROJECT_SOURCE_DIR}/lib/app
  PRIVATE ${spdlog_DIR}/include
  PRIVATE ${LIBCZI_INCLUDE_DIR}
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
