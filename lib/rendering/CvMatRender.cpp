#include "CvMatRender.hpp"

#include <memory>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

#include "imaging/Image.hpp"

using namespace pixelarium::imaging;

pixelarium::render::CvMatRender::CvMatRender(const std::shared_ptr<PixelariumImage>& img) : base_(img), texture_(0)
{
    // storing a copy of the to-be-rendered image
    // because it will be resized and filtered eventually which we absolutely
    // must not do on the original image
    this->img_ = this->base_->GetImage().clone();
    cv::cvtColor(this->img_, this->img_, cv::COLOR_BGR2RGBA);
}

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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    const int width = img_.cols;
    const int height = img_.rows;

    GLenum format = (img_.type() == CV_32FC3 || img_.type() == CV_32FC1) ? GL_RGB : GL_RGBA;
    GLenum type = (img_.type() == CV_16U || img_.type() == CV_16UC3) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_BYTE;
    GLenum internalFormat = GL_RGBA;
    if (img_.type() == CV_32FC3 || img_.type() == CV_32FC1)
    {
        internalFormat = GL_RGB;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, img_.data);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        throw std::runtime_error("OpenGL error during texture upload: " + std::to_string(error));
    }

    return this->texture_;
}

GLuint pixelarium::render::CvMatRender::Render() { return this->uploadTexture(); }

GLuint pixelarium::render::CvMatRender::Render(float factor)
{
    cv::resize(this->base_->GetImage(), this->img_, cv::Size(0, 0), factor, factor, cv::INTER_LINEAR_EXACT);

    return this->uploadTexture();
}

GLuint pixelarium::render::CvMatRender::Render(size_t width, size_t height)
{
    const auto sz{this->base_->GetImage().size()};

    const auto get_factor = [](auto opt1, auto opt2) -> float { return opt1 < opt2 ? opt1 : opt2; };

    auto factor = get_factor(width / static_cast<float>(sz.width), height / static_cast<float>(sz.height));
    // cv::resize(this->base_->GetImage(), this->img_, cv::Size(0, 0), factor, factor, cv::INTER_LINEAR_EXACT);

    return this->Render(factor);
}
