set(RENDERLIBSRC
  RenderHelpers.hpp
  RenderHelpers.cpp
  CvMatRender.hpp
  CvMatRender.cpp
  RenderImageManager.hpp
  RenderImageManager.cpp
  IPixelariumImageView.hpp
  PixelariumImageViewDefault.hpp
  PixelariumImageViewDefault.cpp
  PixelariumImageViewCzi.hpp
  PixelariumImageViewCzi.cpp
  ImageViewFactory.hpp
  ImageViewFactory.cpp)

add_library(${PIXELARIUM_RENDER_LIB_NAME} STATIC
  ${RENDERLIBSRC})

target_link_libraries(${PIXELARIUM_RENDER_LIB_NAME}
  PRIVATE pixelariumimagelib)

target_include_directories(${PIXELARIUM_RENDER_LIB_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/lib
  PRIVATE ${imgui_DIR})
