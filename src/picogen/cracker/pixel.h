#ifndef PIXEL_H
#define PIXEL_H

#include "color.h"

namespace picogen { namespace cracker {
        class Pixel {
        public:
                Color color() const {
                        return color_;
                }

                void setColor (Color const & color) {
                        color_ = color;
                }
        private:
                Color color_;
        };
} }

#endif // PIXEL_H
