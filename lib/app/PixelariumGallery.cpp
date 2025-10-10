#include "PixelariumGallery.hpp"

#include <format>

#include "app_resources_default.h"
#include "imgui.h"

using namespace pixelarium::application;

void PixelariumImageGallery::RenderGallery()
{
    ImGui::SetNextWindowSize(ImVec2(300, 550));
    ImGui::Begin(SHOWIMAGEGALLERY, &this->image_listp_);

    // this updates the render collection
    // essentially deleting render views that were
    // marked for deletion
    this->render_manager_->UpdateCollection();

    static size_t selected_index{0};
    int highlight_index{-1};

    if (ImGui::BeginListBox("Image List", ImVec2(200, 400)))
    {
        pool_.EnumerateResources(
            [&](size_t id, size_t idx, const imaging::IPixelariumImage& img) -> void
            {
                const bool is_selected = selected_index == idx;
                if (ImGui::Selectable(std::format("{}", img.Name()).c_str(), is_selected))
                {
                    selected_index = idx;
                    this->selected_image_ = id;
                }
                if (highlight_index && ImGui::IsItemHovered()) highlight_index = idx;

                if (is_selected) ImGui::SetItemDefaultFocus();
            });

        ImGui::EndListBox();
    }

    ImGui::Checkbox(AUTOSHOWSELECTED, &this->auto_show_selectd_image_);
    ImGui::SameLine();  // put the button next to the checkbox
    // note that the button will only show when the checkbox is toggled off
    // this is intended behavior as the selected image will render automatically
    // when the checkbox is toggled on
    if (this->auto_show_selectd_image_ || ImGui::Button(OPENIMAGE))
    {
        // Try add the selected index to the manager
        this->render_manager_->Add(this->selected_image_);
    }

    if (ImGui::Button(LOADIMAGE))
    {
        this->load_image_();
    }

    if (ImGui::Button(REMOVEIMAGE))
    {
        this->render_manager_->MarkForDeletion(this->selected_image_);
        this->pool_.DeleteResource(this->selected_image_);
    }

    ImGui::SameLine();
    if (ImGui::Button(CLEARALL))
    {
        this->render_manager_->Clear();
        this->pool_.Clear();
    }

    ImGui::End();  // end gallery window
}

void PixelariumImageGallery::RenderImages()
{
    this->render_manager_->Enumerate(
        [&](resources::ResourceKey key, render::RenderImageStateWrapper& render_state)
        {
            render_state.view->ShowImage();

            if (!*render_state.view->GetStatus())
            {
                this->render_manager_->MarkForDeletion(key);
            }
        });
}
