#include <Motor/Engine/Window.h>
#include <Motor/Engine/Engine.h>

#include <exception>

using namespace motor;

Window::~Window() { }

void Window::onCreate() {
    auto rc = getClientRect();
    m_swapChain = m_engine->getGraphicsSystem()->getEngine()->getRenderSystem()->createSwapChain((HWND)m_handle, rc.width, rc.height);
}

void Window::onDestroy() {
}

void Window::update() {
    RenderSystem* render_system = m_engine->getGraphicsSystem()->getEngine()->getRenderSystem();
    render_system->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swapChain, 0, 0.3f, 0.4f, 1);

    auto rc = this->getClientRect();
    render_system->getImmediateDeviceContext()->setViewportSize(rc.width, rc.height);

    m_swapChain->present(true);
}

void Window::onFocus() {
}

void Window::onKillFocus() {
}

void Window::onSize() {
    auto rc = getClientRect();
    if (m_swapChain.get()) {
        m_swapChain->resize(rc.width, rc.height);
    }

    m_engine->update();
}
