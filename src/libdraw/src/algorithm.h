#ifndef ALGORITHM_H_20120413
#define ALGORITHM_H_20120413

namespace draw {

        // For each pixel P, set P = f(P).
        // fun shall be pure.
        template <typename Surface, typename Fun>
        inline
        void transform (Surface &target, Fun f)
        {
                const int w = target.width(),
                          h = target.height();
                for (int y=0; y!=h; ++y) {
                        for (int x=0; x!=w; ++x) {
                                auto index = target.index(x,y);
                                target(index) = f(target(index));
                        }
                }
        }

        // For each pixel P, set P = f().
        // fun shall be pure.
        template <typename Surface, typename Fun>
        inline
        void generate (Surface &target, Fun f)
        {
                const int w = target.width(),
                          h = target.height();
                for (int y=0; y!=h; ++y) {
                        for (int x=0; x!=w; ++x) {
                                auto index = target.index(x,y);
                                target(index) = f();
                        }
                }
        }

        // For each pixel P, set P=C
        template <typename Surface>
        inline
        void fill (Surface &target, typename Surface::value_type C)
        {
                const int w = target.width(),
                          h = target.height();
                for (int y=0; y!=h; ++y)
                        for (int x=0; x!=w; ++x)
                                target(x,y) = C;
        }

        // For each pixel (x,y), set (x,y)=f(x,y)
        template <typename Surface, typename Fun>
        inline
        void map_xy (Surface &target, Fun f)
        {
                const int w = target.width(),
                          h = target.height();
                for (int y=0; y!=h; ++y) {
                        for (int x=0; x!=w; ++x) {
                                auto index = target.index(x,y);
                                target(index) = f(x,y);
                        }
                }
        }

        // For each pixel (x,y) on surface (w,h), set (x,y)=f(x/w,y/h)
        // Calculation of x/w and y/h is implementation defined.
        template <typename Surface, typename Fun>
        inline
        void map_uv (Surface &target, Fun f)
        {
                const int w = target.width(),
                          h = target.height();
                float u = 0, v = 0;
                const float ud = 1.f / w,
                            vd = 1.f / h;

                for (int y=0; y!=h; ++y, v+=vd) {
                        for (int x=0; x!=w; ++x, u+=ud) {
                                auto index = target.index(x,y);
                                target(index) = f(u,v);
                        }
                }
        }
}

#endif // ALGORITHM_H_20120413
