#ifndef PATH_H_20110725
#define PATH_H_20110725

namespace picogen { namespace cracker {

class Ray;
class Scene;
class Random;
class Color;

class PathIntegrator {
public:
        Color operator() (Ray const &,
                          Scene const &,
                          Random &) const;
};

} }

#endif // PATH_H_20110725
