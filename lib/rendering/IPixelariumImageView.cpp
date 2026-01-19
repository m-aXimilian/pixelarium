#include "IPixelariumImageView.hpp"

#include <opencv2/imgcodecs.hpp>

#include "portable-file-dialogs.h"

using namespace pixelarium::render;
void IPixelariumImageView::ImageViewMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::MenuItem("Save As"))
        {
            auto dest = pfd::save_file("Save File", ".", {"Image Files", "*.png *.jpg *.jpeg *.tiff"},
                                       pfd::opt::force_overwrite)
                            .result();
            if (!dest.empty())
            {
                // this->img_->SaveImage(dest);
                cv::imwrite(dest, cached_image_);
            }
        }

        ImGui::EndMenuBar();
    }
}
