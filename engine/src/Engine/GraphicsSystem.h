#pragma once

#include <Motor/Renderer/RHI/GraphicsEngine.h>

class GraphicsSystem {
private:
    GraphicsSystem();
    ~GraphicsSystem();
    static void destroy();

public:
    virtual void release();
    static GraphicsSystem* create();

    GraphicsEngine* getEngine();

private:
    GraphicsEngine* m_engine = nullptr;

private:
    static GraphicsSystem* m_instance;
};
