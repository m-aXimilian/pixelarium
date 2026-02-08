set(SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/custom_0.cpp)

set(CUSTOM_0_NAME "${PROJECT_NAME}_custom_app")

add_executable(${CUSTOM_0_NAME} ${SRC})

target_link_libraries(${CUSTOM_0_NAME}
  PRIVATE pixelarium::lib::application_static)
