#include "CvMatRender.hpp"

#include <cstdint>
#include <memory>
#include <opencv2/core/mat.hpp>
#include <tuple>
#include <utility>
#include "imaging/Image.hpp"

using namespace pixelarium::imaging;

pixelarium::render::CvMatRender::CvMatRender(const std::shared_ptr<Image>& img)
    : _base(img), _texture(0)
{
    this->_img = this->_base->GetImage().clone();
    // // storing a copy of the to-be-rendered image with a "well-behaved"
    // cv::cvtColor(this->_img, this->_img, cv::COLOR_BGR2RGBA);
}

/*static*/ void pixelarium::render::matToTexture(const cv::Mat& image,
                                                 GLuint* texture)
{
    // only generate the texture when it's not already present
    if (*texture == 0)
    {
        glGenTextures(1, texture);
    }

    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // auto image = img.GetImage();
    switch (image.type())
    {
        case CV_16U:
        case CV_16UC3:
        case 26:
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.cols, image.rows, 0,
                         GL_RGBA, GL_UNSIGNED_SHORT, image.data);
            break;
        case 5:
        case 29:
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.cols, image.rows, 0,
                         GL_RGBA, GL_FLOAT, image.data);
            break;
        default:
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.cols, image.rows, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, image.data);
            break;
    }
}

GLuint* pixelarium::render::CvMatRender::Render()
{
    // storing a copy of the to-be-rendered image with a "well-behaved"
    cv::cvtColor(this->_img, this->_img, cv::COLOR_BGR2RGBA);

    matToTexture(this->_img, &this->_texture);
    return &this->_texture;
}

GLuint* pixelarium::render::CvMatRender::Render(float factor)
{
    cv::resize(this->_base->GetImage(), this->_img, cv::Size(0,0), factor, factor, cv::INTER_LINEAR_EXACT);

    return this->Render();
}

GLuint* pixelarium::render::CvMatRender::Render(size_t width, size_t height)
{
    // this is nasty as it knows about what Render is doing
    const auto sz{this->_base->GetImage().size()};

    const auto get_factor = [](auto opt1, auto opt2) -> float
    {
        return opt1 < opt2 ? opt1 : opt2;
    };

    auto factor = get_factor(width / static_cast<float>(sz.width), height / static_cast<float>(sz.height));
    // cv::resize(this->_base->GetImage(), this->_img, cv::Size(0,0), factor, factor, cv::INTER_LINEAR_EXACT);

    return this->Render(factor);
}

