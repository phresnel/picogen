#ifndef PRESENTATIONFILTER_H_INCLUDED_20111007
#define PRESENTATIONFILTER_H_INCLUDED_20111007

namespace picogen { namespace cracker {

        class Color;

        class PresentationFilter {
        public:
                Color operator() (Color const &in) const;

                //TODO: void add_function (std::function ...
        };

} }

#endif // PRESENTATIONFILTER_H_INCLUDED_20111007
