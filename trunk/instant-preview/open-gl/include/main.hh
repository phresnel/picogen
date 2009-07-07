
#ifndef MAIN_HH_INCLUDED_20090704
#define MAIN_HH_INCLUDED_20090704

namespace instant_preview {
        class HeightFunction {
        public:
                virtual float operator () (float, float) const = 0;
        };
        
        void draw (const HeightFunction &);
}


#endif // MAIN_HH_INCLUDED_20090704
