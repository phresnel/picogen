#ifndef MATERIAL_H_20110720
#define MATERIAL_H_20110720

#include "color.h"
#include "math3d/direction.h"

namespace picogen { namespace cracker {
class Random;


class InDirection {
public:
        InDirection() = delete;
        InDirection(Direction const &vec) : vec_(vec) {}
        Direction direction() const { return vec_; }
private:
        Direction vec_;
};

class OutDirection {
public:
        OutDirection() = delete;
        OutDirection(Direction const &vec) : vec_(vec) {}
        Direction direction() const { return vec_; }
private:
        Direction vec_;
};


class Material {
public:
        Material () : whittedMirror_(false) {}

        Color::Optional brdf (InDirection const &in,
                              OutDirection const &out,
                              Random &rand) const
        {
                return this->brdf_(in, out, rand);
        }

        bool whittedMirror() const { return whittedMirror_; }

protected:
        Material(bool mirror) : whittedMirror_(mirror)
        {}

private:
        bool whittedMirror_;
        virtual Color::Optional brdf_ (InDirection const &in,
                                       OutDirection const &out,
                                       Random &rand) const = 0;
};

} }

#endif //  MATERIAL_H_20110720
