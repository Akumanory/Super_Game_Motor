#pragma once

#include <Motor/Renderer/Renderer.h>

#include <SimpleMath.h>

namespace motor {

class Engine;
class Window {
public:
    Window(Engine* engine);
    ~Window();

    virtual DirectX::SimpleMath::Rectangle getClientRect() = 0;
    virtual DirectX::SimpleMath::Rectangle getWindowRect() = 0;

    virtual void onCreate();
    virtual void onDestroy();
    virtual void onFocus();
    virtual void onKillFocus();
    virtual void onSize();

    void update();
    virtual void release();

protected:
    void* m_handle = nullptr;
    SwapChainPtr m_swapChain;
    Engine* m_engine;
};
} // namespace motor
