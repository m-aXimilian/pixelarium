#pragma once

#include "rendering/RenderImageManager.hpp"
#include "resources/resource.hpp"
#include "utilities/ILog.hpp"
namespace pixelarium::application
{

template <typename P>
concept GalleryT = requires(P& r) { static_cast<resources::IResourcePool<P>&>(r); };

template <typename GalleryT>
class IPixelariumGallery
{
   public:
    virtual ~IPixelariumGallery() = default;
    virtual void RenderGallery() = 0;
};

class PixelariumImageGallery : IPixelariumGallery<resources::ImageResourcePool>
{
    using Pool = resources::ImageResourcePool;
    using Log = utils::log::ILog;
   public:
    PixelariumImageGallery(const Log& log, resources::ImageResourcePool& pool) : pool_{pool}, log_{log}, render_manager_(std::make_unique<render::RenderImageManager>(pool, log)) {}

    void RenderGallery() override;

    void RenderImages();

    void Add(resources::ResourceKey key)
    {
        this->render_manager_->Add(key);
    }
    
    void SetLoadFunction(const std::function<void()>& fun) {this->load_image_ = fun; };
   private:
    std::function<void()> load_image_ {};
    Pool& pool_;
    const Log& log_;
    std::unique_ptr<render::RenderImageManager> render_manager_;
    bool image_listp_{true};
    bool auto_show_selectd_image_{true};
    size_t selected_image_{0};
};
}  // namespace pixelarium::application
