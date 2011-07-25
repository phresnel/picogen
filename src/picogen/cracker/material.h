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



class BsdfSample {
public:
        static BsdfSample Null () {
                return BsdfSample (Direction(0,1,0), 0, Color::Black());;
        }

        BsdfSample () = delete;

        BsdfSample(InDirection const &incident,
                   real pdf,
                   Color const &reflection)
                : incident_(incident)
                , pdf_(pdf)
                , reflection_(reflection)
        {
                assert (pdf>=0 && pdf<=1);
        }

        InDirection incident()   const { return incident_;   }
        real        pdf()        const { return pdf_;        }
        Color       reflection() const { return reflection_; }

private:
        InDirection incident_;
        real pdf_;
        Color reflection_;
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

        BsdfSample sample(OutDirection const &out, Random &rand) const {
                return this->sample_(out, rand);
        }

        // TODO: maybe recoin it "deterministic_brdf" or so
        bool whittedMirror() const { return whittedMirror_; }

protected:
        Material(bool mirror) : whittedMirror_(mirror)
        {}

private:
        bool whittedMirror_;
        virtual Color::Optional brdf_ (InDirection const &in,
                                       OutDirection const &out,
                                       Random &rand) const = 0;
        virtual BsdfSample sample_ (OutDirection const &,
                                    Random &rand) const = 0;
};

} }

#endif //  MATERIAL_H_20110720
