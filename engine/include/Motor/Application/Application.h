#pragma once

#include <Motor/Engine/Core.h>
#include <Motor/Engine/Engine.h>

namespace motor {
class Application {
public:
    Application();
    virtual ~Application();

    virtual void onInit() { }
    virtual void onQuit() { }
    virtual void onUpdate() { }

    void run();
    void quit();

    Engine* getEngine();

public:
    static Application* get();

private:
    static Application* m_instance;

private:
    bool m_isRunning = true;
    Engine* m_engine = nullptr;
};
} // namespace motor