#include "IPixelariumImageView.hpp"

#include <opencv2/imgcodecs.hpp>

#include "app_resources_default.h"
#include "portable-file-dialogs.h"

auto pixelarium::application::IPixelariumImageView::ImageViewMenuBar() -> void
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::MenuItem(SAVEAS))
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
