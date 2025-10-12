#pragma once

#include "rendering/RenderImageManager.hpp"
#include "resources/resource.hpp"
#include "utilities/ILog.hpp"
namespace pixelarium::application
{

/// @brief Defines a concept for a gallery type
/// @tparam P  The resource pool type of the gallery concept
template <typename P>
concept GalleryT = requires(P& p) { static_cast<resources::IResourcePool<P>&>(p); };

/// @brief Interface for a Pixelarium gallery.
///
/// Defines generic functionality for a gallery of a specific
/// resource type given by the template argument.
/// @tparam GalleryT  The type of IResourcePool that the given implementation
///                   provides a gallery for.
template <typename GalleryT>
class IPixelariumGallery
{
   public:
    virtual ~IPixelariumGallery() = default;
    virtual void RenderGallery() = 0;
};

/// @brief Implements IPixelariumGallery for a ImageResourcePool
class PixelariumImageGallery : IPixelariumGallery<resources::ImageResourcePool>
{
    using Pool = resources::ImageResourcePool;
    using Log = utils::log::ILog;

   public:
    PixelariumImageGallery(const Log& log, resources::ImageResourcePool& pool)
        : pool_{pool}, log_{log}, render_manager_(std::make_unique<render::RenderImageManager>(pool, log))
    {
    }

    void RenderGallery() override;

    void RenderImages();

    void Add(resources::ResourceKey key) { this->render_manager_->Add(key); }

    void SetLoadFunction(const std::function<void()>& fun) { this->load_image_ = fun; };

   private:
    std::function<void()> load_image_{};
    Pool& pool_;
    const Log& log_;
    std::unique_ptr<render::RenderImageManager> render_manager_;
    bool image_listp_{true};
    bool auto_show_selectd_image_{true};
    size_t selected_image_{0};
};
}  // namespace pixelarium::application
