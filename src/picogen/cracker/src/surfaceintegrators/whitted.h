#ifndef WHITTED_H_20110708
#define WHITTED_H_20110708

namespace picogen { namespace cracker {

class Ray;
class Scene;
class Random;
class Color;
class RendererBase;
class Intersection;

class WhittedIntegrator {
public:
        Color operator() (Ray const &,
                          Intersection const &,
                          Scene const &,
                          RendererBase const &,
                          Random &) const;
};

} }

#endif // RAYDIRECTION_H_20110708
