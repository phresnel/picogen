#ifndef PIXEL_H
#define PIXEL_H

#include "color.h"

namespace picogen { namespace cracker {
        class Pixel {
        public:
                Color color() const {
                        if (numSamples_ == 0)
                                return Color::Black();
                        return color_ / numSamples_;
                }

                unsigned int numSamples() const {
                        return numSamples_;
                }

                void add (Color const & color) {
                        color_ += color;
                        ++numSamples_;
                }
        private:
                Color color_;
                unsigned int numSamples_;
        };
} }

#endif // PIXEL_H
