#pragma once
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

namespace pixelarium::render
{
    static void matToTexture(const cv::Mat& image,
                         GLuint* texture);
class CvMatRender
{
   public:
    explicit CvMatRender(const pixelarium::imaging::Image& img);
    // void* Render();
    GLuint Render();

   private:
    cv::Mat _img;
    GLuint _texture;
};

}  // namespace pixelarium::render