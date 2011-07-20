#ifndef MATERIAL_H_20110720
#define MATERIAL_H_20110720

#include "color.h"
#include "math3d/vector.h"

namespace picogen { namespace cracker {
class Random;


class InVector {
public:
        InVector() = delete;
        InVector(Vector const &vec) : vec_(vec) {}
        Vector vector() const { return vec_; }
private:
        Vector vec_;
};

class OutVector {
public:
        OutVector() = delete;
        OutVector(Vector const &vec) : vec_(vec) {}
        Vector vector() const { return vec_; }
private:
        Vector vec_;
};


class Material {
public:
        Color::Optional brdf (InVector const &in,
                              OutVector const &out,
                              Random &rand) const
        {
                return this->brdf_(in, out, rand);
        }

private:
        virtual Color::Optional brdf_ (InVector const &in,
                                       OutVector const &out,
                                       Random &rand) const = 0;
};

} }

#endif //  MATERIAL_H_20110720
