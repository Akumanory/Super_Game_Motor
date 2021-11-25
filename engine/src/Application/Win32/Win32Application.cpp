#include <Motor/Application/Application.h>

#include <Windows.h>

using namespace motor;

void Application::run() {
    onInit();

    while (m_isRunning && m_engine->getNumWindows()) {
        MSG msg;
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        m_engine->update();
        onUpdate();
        Sleep(1);
    }

    onQuit();
}