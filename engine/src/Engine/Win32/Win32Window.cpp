#include <Motor/Engine/Window.h>

#include <exception>
#include <Windows.h>

using namespace motor;
using namespace DirectX::SimpleMath;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
    case WM_CREATE: {
        // Event fired when the window is created
        Window* window = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;
        // .. and then stored for later lookup
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
        break;
    }
    case WM_SIZE: {
        // Event fired when the window is resized
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (window)
            window->onSize();
        break;
    }
    case WM_SETFOCUS: {
        // Event fired when the window get focus
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (window)
            window->onFocus();
        break;
    }
    case WM_KILLFOCUS: {
        // Event fired when the window lost focus
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (window)
            window->onKillFocus();
        break;
    }
    case WM_DESTROY: {
        // Event fired when the window is destroyed
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (window)
            window->onDestroy();
        break;
    }

    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }

    return NULL;
}

Window::Window(Engine* engine)
    : m_engine(engine) {
    //Setting up WNDCLASSEX object

    WNDCLASSEX wc;
    wc.cbClsExtra = NULL;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = NULL;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = NULL;
    wc.lpszClassName = L"WindowMotorClass";
    wc.lpszMenuName = L"";
    wc.style = NULL;
    wc.lpfnWndProc = &WndProc;

    WNDCLASSEX wc_temp;
    if (!GetClassInfoEx(GetModuleHandle(nullptr), L"WindowMotorClass", &wc_temp)) {
        assert(RegisterClassEx(&wc)); // If the registration of class fails, the function returns false
    }

    //Creation of the window
    m_handle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"WindowMotorClass", L"Super Game Motor",
      WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
      NULL, NULL, GetModuleHandle(nullptr), this);

    //if the creation fail return false
    assert(m_handle);

    //show up the window
    ShowWindow((HWND)m_handle, SW_SHOW);
    UpdateWindow((HWND)m_handle);

    onCreate();
}

Rectangle Window::getClientRect() {
    RECT rc;
    ::GetClientRect((HWND)m_handle, &rc);
    ::ClientToScreen((HWND)m_handle, (LPPOINT)&rc.left);
    ::ClientToScreen((HWND)m_handle, (LPPOINT)&rc.right);

    return Rectangle(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
}

Rectangle Window::getWindowRect() {
    RECT rc;
    GetWindowRect((HWND)m_handle, &rc);
    return Rectangle(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
}

void Window::release() {
    DestroyWindow((HWND)m_handle);
}