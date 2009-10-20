//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
// * mail: phresnel/at/gmail/dot/com
// * http://phresnel.org
// * http://picogen.org
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef NODEITEM_HH_INCLUDED_20091020
#define NODEITEM_HH_INCLUDED_20091020

#include "kallisto/common.hh"
#include "redshift/include/tuple.hh"
#include "actuarius/actuarius.hh"

enum NodeItemType {
        Undefined,
        Parameter,
        UserConstant,
        PredefinedConstant,

        // +-*/
        Addition,
        Subtraction,
        Multiplication,
        Division,

        // ^ min max neg
        Exponentiate,
        Minimize,
        Maximize,
        Negate,

        // lerp
        Lerp,

        // and or not
        And,
        Or,
        Not,

        // < <= > >= = !=
        LessThan,
        LessThanOrEqual,
        GreaterThan,
        GreaterThanOrEqual,
        //Equal,
        //NotEqual,

        // [] ]] ][ [[

        // inv sin cos
        Inverse,
        Sine,
        Cosine,

        // floor abs trunc frac
        Floor,
        Absolute,
        Truncate,
        Fractional,

        // sqrt log log10 exp
        Sqrt,
        Log,
        Log10,
        Exp,

        // delta

        // if
        IfThenElse,

        // [configurable]
        Noise2d,
        LayeredNoise2d,

        // mulpi
        MultiplyWithPi
};

class NodeItemValue {
public:
        enum PredefinedConstant {
                Pi, e
        };

        enum ScaleOffsetMode {
                disable,
                scale_offset,
                offset_scale
        };

        NodeItemValue ()
        : predefinedConstant (Pi)
        , floatConstant (0.0f)
        , parameter ("x")
        , noise2d ()
        , layeredNoise2d ()
        , scaleOffsetMode (disable)
        , scale (1.0f)
        , offset (0.0f)
        {}

        NodeItemValue &operator = (NodeItemValue const &val) {
                predefinedConstant = val.predefinedConstant;
                floatConstant = val.floatConstant;
                parameter = val.parameter;
                noise2d = val.noise2d;
                layeredNoise2d = val.layeredNoise2d;
                scaleOffsetMode = val.scaleOffsetMode;
                scale = val.scale;
                offset = val.offset;
                return *this;
        }

        NodeItemValue (NodeItemValue const &val)
        : predefinedConstant (val.predefinedConstant)
        , floatConstant (val.floatConstant)
        , parameter (val.parameter)
        , noise2d (val.noise2d)
        , layeredNoise2d (val.layeredNoise2d)
        , scaleOffsetMode (val.scaleOffsetMode)
        , scale (val.scale)
        , offset (val.offset)
        {
        }

        PredefinedConstant asPredefinedConstant () const
        { return predefinedConstant; }

        void setPredefinedConstant (PredefinedConstant val)
        { predefinedConstant = val; }

        float asFloatConstant () const { return floatConstant; }
        void setFloatConstant (float val) { floatConstant = val; }

        std::string asParameter () const { return parameter; }
        void setParameter (std::string p) { parameter = p; }


        struct Noise2d {
                enum Filter { Nearest, Bilinear, Cosine };

                Filter filter;
                uint32_t seed;
                uint32_t width;

                Noise2d () : filter(Cosine), seed(42), width(16) {}
                Noise2d (Noise2d const&val)
                : filter(val.filter)
                , seed (val.seed)
                , width (val.width)
                {}

                Noise2d &operator = (Noise2d const &val) {
                        filter = val.filter;
                        seed = val.seed;
                        width = val.width;
                        return *this;
                }

                template<typename Archive>
                void serialize (Archive &arch) {
                        using namespace actuarius;

                        const Enum<Filter> filterTypeNames =
                        (Nvp<Filter>(Nearest,"Nearest")
                        |Nvp<Filter>(Bilinear,"Bilinear")
                        |Nvp<Filter>(Cosine,"Cosine")
                        );

                        arch
                        & pack ("filter", filter, filterTypeNames)
                        & pack ("seed", seed)
                        & pack ("width", width)
                        ;
                }
        };

        struct LayeredNoise2d {
                enum Filter { Nearest, Bilinear, Cosine };

                Filter filter;
                uint32_t seed;
                uint32_t width;
                uint32_t depth;

                LayeredNoise2d () : filter(Cosine), seed(42), width(16), depth(4) {}
                LayeredNoise2d (LayeredNoise2d const&val)
                : filter(val.filter)
                , seed (val.seed)
                , width (val.width)
                , depth (val.depth)
                {}

                LayeredNoise2d &operator = (LayeredNoise2d const &val) {
                        filter = val.filter;
                        seed = val.seed;
                        width = val.width;
                        depth = val.depth;
                        return *this;
                }

                template<typename Archive>
                void serialize (Archive &arch) {
                        using namespace actuarius;

                        const Enum<Filter> filterTypeNames =
                        (Nvp<Filter>(Nearest,"Nearest")
                        |Nvp<Filter>(Bilinear,"Bilinear")
                        |Nvp<Filter>(Cosine,"Cosine")
                        );

                        arch
                        & pack ("filter", filter, filterTypeNames)
                        & pack ("seed", seed)
                        & pack ("width", width)
                        & pack ("depth", depth)
                        ;
                }
        };

        Noise2d asNoise2d () const { return noise2d; }
        void setNoise2d (Noise2d const &noise2d) {
                this->noise2d = noise2d;
        }

        LayeredNoise2d asLayeredNoise2d () const { return layeredNoise2d; }
        void setLayeredNoise2d (LayeredNoise2d const &layeredNoise2d) {
                this->layeredNoise2d = layeredNoise2d;
        }

        double getScale () const {
                return scale;
        }
        void setScale (double scale) {
                this->scale = scale;
        }
        double getOffset () const {
                return offset;
        }
        void setOffset (double offset) {
                this->offset = offset;
        }
        ScaleOffsetMode getScaleOffsetMode () const {
                return scaleOffsetMode;
        }
        void setScaleOffsetMode (ScaleOffsetMode som) {
                scaleOffsetMode = som;
        }



        template<typename Archive>
        void serialize (Archive &arch) {
                using namespace actuarius;

                const Enum<ScaleOffsetMode> scaleOffsetModeTypeNames =
                (Nvp<ScaleOffsetMode>(disable,"disable")
                |Nvp<ScaleOffsetMode>(scale_offset,"scale_offset")
                |Nvp<ScaleOffsetMode>(offset_scale,"offset_scale")
                );

                const Enum<PredefinedConstant> predefinedConstantTypeNames =
                (Nvp<PredefinedConstant>(Pi,"Pi")
                |Nvp<PredefinedConstant>(e,"e")
                );

                arch
                & pack ("predefinedConstant",predefinedConstant,predefinedConstantTypeNames)
                & pack ("floatConstant",floatConstant)
                & pack ("parameter",parameter)
                & pack ("noise2d",noise2d)
                & pack ("layeredNoise2d",layeredNoise2d)
                & pack ("scaleOffsetMode",scaleOffsetMode, scaleOffsetModeTypeNames)
                & pack ("scale",scale)
                & pack ("offset",offset)
                ;
        }

private:
        PredefinedConstant predefinedConstant;
        double floatConstant;
        std::string parameter;
        Noise2d noise2d;
        LayeredNoise2d layeredNoise2d;

        ScaleOffsetMode scaleOffsetMode;
        double scale, offset;
};

#endif // NODEITEM_HH_INCLUDED_20091020
