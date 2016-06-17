#include "ork/resource/XMLResourceLoader.h"
#include "ork/resource/ResourceManager.h"
#include "ork/render/FrameBuffer.h"
#include "ork/ui/GlutWindow.h"

using namespace ork;

class SimpleExample : public GlutWindow
{
public:
    ptr<ResourceManager> resManager;
    ptr<MeshBuffers> m;
    ptr<Program> p;

    SimpleExample() : GlutWindow(Window::Parameters())
    {
        ptr<XMLResourceLoader> resLoader = new XMLResourceLoader();
        resLoader->addPath("resources/textures");
        resLoader->addPath("resources/shaders");
        resLoader->addPath("resources/meshes");

        resManager = new ResourceManager(resLoader);
        m = resManager->loadResource("quad.mesh").cast<MeshBuffers>();
        p = resManager->loadResource("basic;").cast<Program>();
    }

    // rest of the code unchanged
};
