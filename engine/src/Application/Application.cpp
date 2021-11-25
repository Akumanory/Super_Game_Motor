#include <Motor/Application/Application.h>

#include <stdexcept>
#include <chrono>
#include <thread>

using namespace motor;

Application* Application::m_instance = nullptr;

Application::Application() {
    assert(!m_instance);
    m_instance = this;
    m_engine = CreateEngine();
}

Application::~Application() {
    m_engine->release();
}

void Application::quit() {
    m_isRunning = false;
}

Engine* Application::getEngine() {
    return m_engine;
}

Application* Application::get() {
    assert(m_instance);
    return m_instance;
}
