#pragma once
// windows.h must come before GL/GL.h here.
// clang format would change this, effectively rendering the build broken.
// clang-format off
#include <memory>
#include <opencv2/core/mat.hpp>
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
#include "imaging/IPixelariumImage.hpp"
// clang-format on

namespace pixelarium::render
{
class CvMatRender
{
   public:
    // we want the default constructor for the time being
    // (although it does not make much sense and should
    // get removed in the future)
    // as the using AppGLFW constructs it empty as a member
    // when coming to life.
    // CvMatRender() = default;
    CvMatRender(CvMatRender&) = delete;
    CvMatRender(const CvMatRender&) = delete;
    CvMatRender(CvMatRender&&) = delete;
    CvMatRender& operator=(CvMatRender&) = delete;
    CvMatRender& operator=(CvMatRender&& other) = delete;
    ~CvMatRender();
    explicit CvMatRender(const cv::Mat& img);

   public:
    GLuint Render();
    GLuint Render(float factor);
    GLuint Render(size_t width, size_t height);
    void ResetRenderImage();

   private:
    cv::Mat img_;
    const cv::Mat& base_;
    GLuint texture_;

    GLuint uploadTexture();
};

}  // namespace pixelarium::render
