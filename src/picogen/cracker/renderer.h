#ifndef RENDERER_H
#define RENDERER_H

#include <memory>

namespace picogen { namespace cracker {

class Scene;
class RenderTarget;

void render(std::shared_ptr<Scene>,
            std::shared_ptr<RenderTarget>);


} }

#endif // RENDERER_H
