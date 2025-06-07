#pragma once
// windows.h must come before GL/GL.h here.
// clang format would change this, effectively rendering the build broken.
// clang-format off
#include <memory>
#ifdef _WIN32
#include <windows.h>
#include <GL/GL.h>
#else
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers
#endif
#include "imaging/Image.hpp"
// clang-format on

namespace pixelarium::render
{
class CvMatRender
{
   public:
    CvMatRender() = default;
    explicit CvMatRender(const std::shared_ptr<pixelarium::imaging::PixelariumImage>& img);
    GLuint Render();
    GLuint Render(float factor);
    GLuint Render(size_t width, size_t height);

   private:
    cv::Mat img_;
    std::shared_ptr<pixelarium::imaging::PixelariumImage> base_;
    GLuint texture_;

    GLuint uploadTexture();
};

}  // namespace pixelarium::render
