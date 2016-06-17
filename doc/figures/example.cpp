#include "ork/render/FrameBuffer.h"
#include "ork/ui/GlutWindow.h"

using namespace ork;

class SimpleExample : public GlutWindow
{
public:
    ptr< Mesh<vec2f, unsigned int> > m;
    ptr<Program> p;

    SimpleExample() : GlutWindow(Window::Parameters())
    {
        m = new Mesh<vec2f, unsigned int>(TRIANGLE_STRIP, GPU_STATIC);
        m->addAttributeType(0, 2, A32F, false);
        m->addVertex(vec2f(-1, -1));
        m->addVertex(vec2f(+1, -1));
        m->addVertex(vec2f(-1, +1));
        m->addVertex(vec2f(+1, +1));

        unsigned char data[16] = {
			0, 255, 0, 255,
			255, 0, 255, 0,
			0, 255, 0, 255,
			255, 0, 255, 0
		};
        ptr<Texture2D> tex = new Texture2D(4, 4, R8, RED, UNSIGNED_BYTE,
            Texture::Parameters().mag(NEAREST), Buffer::Parameters(), CPUBuffer(data));

        p = new Program(new Module(330, NULL, "\
            #version 330\n\
            uniform sampler2D sampler;\n\
            uniform vec2 scale;\n\
            layout(location = 0) out vec4 data;\n\
            void main() {\n\
                data = texture(sampler, gl_FragCoord.xy * scale).rrrr;\n\
            }\n"));

        p->getUniformSampler("sampler")->set(tex);
    }

    virtual void redisplay(double t, double dt)
    {
        ptr<FrameBuffer> fb = FrameBuffer::getDefault();
        fb->clear(true, false, false);
        fb->draw(p, *m);
        GlutWindow::redisplay(t, dt);
    }

    virtual void reshape(int x, int y)
    {
        FrameBuffer::getDefault()->setViewport(vec4<GLint>(0, 0, x, y));
        p->getUniform2f("scale")->set(vec2f(1.0f / x, 1.0f / y));
        GlutWindow::reshape(x, y);
        idle(false);
    }
};

int main(int argc, char** argv)
{
    atexit(Object::exit);
    ptr<SimpleExample> app = new SimpleExample();
    app->start();
    return 0;
}
