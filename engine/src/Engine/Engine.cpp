#include <Motor/Engine/Engine.h>

using namespace motor;

Engine* Engine::m_instance = nullptr;

Engine::Engine() {
    m_graphicsSystem = GraphicsSystem::create();
}

Engine::~Engine() {
    m_graphicsSystem->release();
}

Engine* Engine::create() {
    assert(!m_instance);

    m_instance = new Engine();
    return m_instance;
}

void Engine::destroy() {
    assert(m_instance);
    delete m_instance;
}

void Engine::release() {
    Engine::destroy();
}

Scene* Engine::createScene() {
    return new Scene();
}

ui32 Engine::getNumScenes() {
    //return m_sceneFactory.getNumResources();
}

Window* Engine::createWindow() {
    //return m_windowFactory.createResource(this);
}

ui32 Engine::getNumWindows() {
    return m_windowFactory.getNumResources();
}

void Engine::update() {
    for (auto scene : m_sceneFactory.m_setResources) {
        scene->update();
    }

    for (auto win : m_windowFactory.m_setResources) {
        win->update();
    }
}

GraphicsSystem* Engine::getGraphicsSystem() {
    return m_graphicsSystem;
}

Engine* CreateEngine() {
    return Engine::create();
}
