#include "RenderImageManager.hpp"

namespace pi = pixelarium;

pi::ui::RenderImageManager::RenderImageManager(Pool& pool) { view_factory_ = std::make_unique<ImageViewFactory>(pool); }

