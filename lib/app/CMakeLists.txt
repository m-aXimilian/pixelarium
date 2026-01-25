# Fetch implot
include(implot)
message(STATUS "IMPLOT sources at ${IMPLOT_DIR}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/app_resources_default.h.in
  ${CMAKE_BINARY_DIR}/app_resources_default.h @ONLY)

set(IMPLOTSRC
  "${IMPLOT_DIR}/implot.h"
  "${IMPLOT_DIR}/implot_internal.h"
  "${IMPLOT_DIR}/implot_items.cpp"
  "${IMPLOT_DIR}/implot.cpp"
  "${IMPLOT_DIR}/implot_demo.cpp"
)

set(IMGUISRC
  "${imgui_DIR}/imgui.cpp"
  "${imgui_DIR}/imgui_demo.cpp"
  "${imgui_DIR}/imgui_draw.cpp"
  "${imgui_DIR}/imgui_tables.cpp"
  "${imgui_DIR}/imgui_widgets.cpp"
  "${imgui_DIR}/backends/imgui_impl_opengl3.cpp"
  "${imgui_DIR}/backends/imgui_impl_glfw.cpp")

set(APPLIBSRC
  AppGLFW.hpp
  AppGLFW.cpp
  DefaultApp.hpp
  DefaultApp.cpp
  PixelariumGallery.hpp
  PixelariumGallery.cpp)

set(RENDERSRC
  rendering/RenderHelpers.hpp
  rendering/RenderHelpers.cpp
  rendering/CvMatRender.hpp
  rendering/CvMatRender.cpp
  rendering/RenderImageManager.hpp
  rendering/RenderImageManager.cpp
  rendering/IPixelariumImageView.hpp
  rendering/IPixelariumImageView.cpp
  rendering/PixelariumImageViewDefault.hpp
  rendering/PixelariumImageViewDefault.cpp
  rendering/PixelariumImageViewCzi.hpp
  rendering/PixelariumImageViewCzi.cpp
  rendering/ImageViewFactory.hpp
  rendering/ImageViewFactory.cpp)

set(APPLIBNAME pixelariumapplicationlib)

add_library(${APPLIBNAME}
  STATIC ${APPLIBSRC} ${IMGUISRC} ${IMPLOTSRC} ${RENDERSRC})

target_link_libraries(${APPLIBNAME}
  PRIVATE pixelariumutilslib
  PRIVATE pixelariumimagelib)

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
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/rendering
  PUBLIC ${pfd_DIR}
  PUBLIC ${imgui_DIR}
  PUBLIC ${imgui_DIR}/backends
  ${IMPLOT_DIR})
