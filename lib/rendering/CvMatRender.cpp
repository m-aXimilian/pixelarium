#include "CvMatRender.hpp"

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

#include "imaging/IPixelariumImage.hpp"

using namespace pixelarium::imaging;

/// @brief Constructor for the CvMatRender class.
/// @param img A shared pointer to the PixelariumImage to be rendered.
pixelarium::render::CvMatRender::CvMatRender(const cv::Mat& img) : base_(img), texture_(0)
{
    // storing a copy of the to-be-rendered image
    // because it will be resized and filtered eventually which we absolutely
    // must not do on the original image
    this->ResetRenderImage();
    cv::cvtColor(this->img_, this->img_, cv::COLOR_BGR2RGBA);
}

/// @brief Destructor for the CvMatRender class.
/// Deallocates the OpenGL texture if it exists.
pixelarium::render::CvMatRender::~CvMatRender()
{
    if (texture_)
    {
        glDeleteTextures(1, &texture_);
        texture_ = 0;
    }
}

/// @brief Uploads the current image data to an OpenGL texture.
/// @return The ID of the uploaded OpenGL texture.
/// @throws std::runtime_error if the image data is empty or if there is an OpenGL error.
GLuint pixelarium::render::CvMatRender::uploadTexture()
{
    if (img_.empty())
    {
        throw std::runtime_error("Image data is empty.");
    }

    if (!this->texture_)
    {
        glGenTextures(1, &this->texture_);
        if (this->texture_ == 0)
        {
            throw std::runtime_error("Failed to generate OpenGL texture.");
        }
    }

    glBindTexture(GL_TEXTURE_2D, this->texture_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    const int width = img_.cols;
    const int height = img_.rows;

    switch (img_.type()) {
        case CV_16U:
        case CV_16UC3:
        case 26:
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT, img_.data);
            break;
        case 5:
        case 29:
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, img_.data);
            break;
        default:
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_.cols, img_.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_.data);
            break;
    }

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        throw std::runtime_error("OpenGL error during texture upload: " + std::to_string(error));
    }

    return this->texture_;
}

/// @brief Renders the image by uploading it as a texture.
/// @return The ID of the OpenGL texture.
GLuint pixelarium::render::CvMatRender::Render() { return this->uploadTexture(); }

/// @brief Renders the image with a specified scaling factor.
/// @param factor The scaling factor for resizing the image.
/// @return The ID of the OpenGL texture.
GLuint pixelarium::render::CvMatRender::Render(float factor)
{
    cv::resize(this->base_, this->img_, cv::Size(0, 0), factor, factor, cv::INTER_LINEAR_EXACT);

    return this->uploadTexture();
}

/// @brief Renders the image to fit within the specified width and height.
/// @param width The maximum width of the rendered image.
/// @param height The maximum height of the rendered image.
/// @return The ID of the OpenGL texture.
GLuint pixelarium::render::CvMatRender::Render(size_t width, size_t height)
{
    const auto sz{this->base_.size()};

    const auto get_factor = [](auto opt1, auto opt2) -> float { return opt1 < opt2 ? opt1 : opt2; };

    auto factor = get_factor(width / static_cast<float>(sz.width), height / static_cast<float>(sz.height));

    return this->Render(factor);
}

void pixelarium::render::CvMatRender::ResetRenderImage()
{
    // we copy here
    this->img_ = this->base_.clone();
}
