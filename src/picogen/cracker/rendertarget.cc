
#include "rendertargetrow.h"
#include "rendertarget.h"

namespace picogen { namespace cracker {

RenderTarget::RenderTarget(unsigned int width,
                           unsigned int height)
: width_(width)
, height_(height)
, pixels_(width*height)
{
}

RenderTargetRow RenderTarget::row (size_t y) {
        return RenderTargetRow (&pixels_[y*width_],
                                width_);
}

ConstRenderTargetRow RenderTarget::row (size_t y) const {
        return ConstRenderTargetRow (&pixels_[y*width_],
                                width_);
}

unsigned int RenderTarget::width () const {
        return width_;
}

unsigned int RenderTarget::height() const {
        return height_;
}


} }
