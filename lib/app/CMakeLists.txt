set(imgui_DIR ${PROJECT_SOURCE_DIR}/modules/imgui)
# set(glfw_DIR ${PROJECT_SOURCE_DIR}/modules/glfw)

message(STATUS "IMGUI:\t" ${imgui_DIR})
# message(STATUS "GLFW:\t" ${glfw_DIR})

# add_directory(${glfw_DIR})

set(PIXELARIUM_TITLE ${CMAKE_PROJECT_NAME})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/uiresources_app.h.in
  ${CMAKE_BINARY_DIR}/uiresources_app.h @ONLY)

set(APPLIBSRC
  AppGLFW.cpp
  ${imgui_DIR}/imgui.cpp
  ${imgui_DIR}/imgui_demo.cpp
  ${imgui_DIR}/imgui_draw.cpp
  ${imgui_DIR}/imgui_tables.cpp
  ${imgui_DIR}/imgui_widgets.cpp
  ${imgui_DIR}/backends/imgui_impl_opengl3.cpp
  ${imgui_DIR}/backends/imgui_impl_glfw.cpp)

set(APPLIBNAME pixelariumapplicationlib)

add_library(${APPLIBNAME}
  STATIC ${APPLIBSRC})

target_link_libraries(${APPLIBNAME}
  PRIVATE pixelariumutilslib)

target_include_directories(${APPLIBNAME}
  PUBLIC ${CMAKE_BINARY_DIR}
  PUBLIC ${PROJECT_SOURCE_DIR}/lib
  PUBLIC ${imgui_DIR}
  PUBLIC ${imgui_DIR}/backends
  PUBLIC ${glfw_INCLUDE_DIR})
