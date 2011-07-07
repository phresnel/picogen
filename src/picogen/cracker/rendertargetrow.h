#ifndef RENDERTARGETROW_H
#define RENDERTARGETROW_H

#include "pixel.h"
#include <cstddef>
#include <cassert>

namespace picogen { namespace cracker {

class RenderTargetRow {
public:
        const Pixel& operator[] (size_t x) const {
                assert (x<width_);
                return row_[x];
        }

        Pixel& operator[] (size_t x) {
                return row_[x];
        }

        RenderTargetRow (Pixel *row,
                         unsigned int width)
        : row_(row)
        , width_ (width)
        {}

private:
        Pixel *row_;
        unsigned int width_;
};

} }

#endif // RENDERTARGETROW_H
