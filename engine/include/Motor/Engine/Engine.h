#pragma once

#include "Scene.h"
#include "Window.h"
#include "GraphicsSystem.h"

namespace motor {
class Engine {
public:
    Engine();
    ~Engine();
    virtual void release() override;

    virtual Scene* createScene() override;
    virtual ui32 getNumScenes() override;

    virtual Window* createWindow() override;
    virtual ui32 getNumWindows() override;

    virtual void update() override;

    GraphicsSystem* getGraphicsSystem();

public:
    static Engine* create();

private:
    static void destroy();

private:
    static Engine* m_instance;
    GraphicsSystem* m_graphicsSystem = nullptr;
};

Engine* CreateEngine();

} // namespace motor