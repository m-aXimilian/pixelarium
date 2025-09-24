configure_file(${CMAKE_CURRENT_SOURCE_DIR}/app_resources_default.h.in
  ${CMAKE_BINARY_DIR}/app_resources_default.h @ONLY)

set(APPLIBSRC
  AppGLFW.hpp
  AppGLFW.cpp
  DefaultApp.hpp
  DefaultApp.cpp
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
  PRIVATE pixelariumutilslib
  PRIVATE pixelariumimagelib
  PRIVATE pixelariumrenderlib)

# This needs to be public to let the consumer know about it.
if(WIN32)
  target_link_libraries(${APPLIBNAME}
    PUBLIC opengl32.lib
    PUBLIC glfw
    PUBLIC OpenGL::GL)
endif()
if(LINUX)
  target_link_libraries(${APPLIBNAME}
    PUBLIC glfw GL)
endif()
if(APPLE)
  target_link_libraries(${APPLIBNAME}
    PUBLIC glfw
    PUBLIC "-framework OpenGL")
endif()

target_include_directories(${APPLIBNAME}
  PRIVATE ${CMAKE_BINARY_DIR}
  PRIVATE ${PROJECT_SOURCE_DIR}/lib
  PRIVATE ${PROJECT_SOURCE_DIR}/lib/imaging
  PUBLIC ${pfd_DIR}
  PUBLIC ${imgui_DIR}
  PUBLIC ${imgui_DIR}/backends)
