#include "GraphicsSystem.h"

GraphicsSystem* GraphicsSystem::m_instance = nullptr;

using namespace motor;

GraphicsSystem::GraphicsSystem() {
    GraphicsEngine::create();
    m_engine = GraphicsEngine::get();
}

GraphicsSystem::~GraphicsSystem() {
    GraphicsEngine::release();
}

GraphicsSystem* GraphicsSystem::create() {
    if (!m_instance) {
        m_instance = new GraphicsSystem();
    }

    return m_instance;
}

void GraphicsSystem::destroy() {
    if (m_instance) {
        delete m_instance;
    }
}

GraphicsEngine* GraphicsSystem::getEngine() {
    return GraphicsEngine::get();
}

void GraphicsSystem::release() {
    GraphicsSystem::destroy();
}
