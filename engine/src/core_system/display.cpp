#include <motor/core_system/display.hpp>
#include <motor/utils.hpp>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include <windowsx.h>
#include <dwmapi.h>
#include <d3d11.h>

using namespace motor;

namespace {
constexpr auto toInputDeviceKey(WPARAM key) -> InputDevice::Key {
    switch (key) {
        // clang-format off
    case VK_LBUTTON:    return InputDevice::Key::MOUSE_BUTTON_LEFT;
    case VK_RBUTTON:    return InputDevice::Key::MOUSE_BUTTON_RIGHT;
    case VK_BACK:       return InputDevice::Key::NONE;
    case VK_TAB:        return InputDevice::Key::NONE;
    case VK_RETURN:     return InputDevice::Key::ENTER;
    case 0x41:          return InputDevice::Key::A;
    case 0x42:          return InputDevice::Key::NONE;
    case 0x43:          return InputDevice::Key::NONE;
    case 0x44:          return InputDevice::Key::D;
    case 0x45:          return InputDevice::Key::E;
    case 0x46:          return InputDevice::Key::NONE;
    case 0x47:          return InputDevice::Key::NONE;
    case 0x48:          return InputDevice::Key::NONE;
    case 0x49:          return InputDevice::Key::NONE;
    case 0x4A:          return InputDevice::Key::NONE;
    case 0x4B:          return InputDevice::Key::NONE;
    case 0x4C:          return InputDevice::Key::NONE;
    case 0x4D:          return InputDevice::Key::NONE;
    case 0x4E:          return InputDevice::Key::NONE;
    case 0x4F:          return InputDevice::Key::NONE;
    case 0x50:          return InputDevice::Key::NONE;
    case 0x51:          return InputDevice::Key::Q;
    case 0x52:          return InputDevice::Key::NONE;
    case 0x53:          return InputDevice::Key::S;
    case 0x54:          return InputDevice::Key::NONE;
    case 0x55:          return InputDevice::Key::NONE;
    case 0x56:          return InputDevice::Key::NONE;
    case 0x57:          return InputDevice::Key::W;
    case 0x58:          return InputDevice::Key::NONE;
    case 0x59:          return InputDevice::Key::NONE;
    case 0x5A:          return InputDevice::Key::NONE;
    case VK_LSHIFT:     return InputDevice::Key::LEFT_SHIFT;
    case VK_RSHIFT:     return InputDevice::Key::RIGHT_SHIFT;
    case VK_LCONTROL:   return InputDevice::Key::LEFT_CTRL;
    case VK_RCONTROL:   return InputDevice::Key::RIGHT_CTRL;
    case VK_LEFT:       return InputDevice::Key::LEFT;
    case VK_UP:         return InputDevice::Key::UP;
    case VK_RIGHT:      return InputDevice::Key::RIGHT;
    case VK_DOWN:       return InputDevice::Key::DOWN;
    default:            return InputDevice::Key::NONE;
        // clang-format on
    }
}

constexpr auto fromInputDeviceKey(InputDevice::Key key) -> unsigned {
    switch (key) {
        // clang-format off
    case InputDevice::Key::MOUSE_BUTTON_LEFT:   return VK_LBUTTON;
    case InputDevice::Key::MOUSE_BUTTON_RIGHT:  return VK_RBUTTON;
    case InputDevice::Key::ENTER:               return VK_RETURN;
    case InputDevice::Key::A:                   return 0x41;
    case InputDevice::Key::D:                   return 0x44;
    case InputDevice::Key::S:                   return 0x53;
    case InputDevice::Key::W:                   return 0x57;
    case InputDevice::Key::E:                   return 0x45;
    case InputDevice::Key::Q:                   return 0x51;
    case InputDevice::Key::LEFT_SHIFT:          return VK_LSHIFT;
    case InputDevice::Key::RIGHT_SHIFT:         return VK_RSHIFT;
    case InputDevice::Key::LEFT_CTRL:           return VK_LCONTROL;
    case InputDevice::Key::RIGHT_CTRL:          return VK_RCONTROL;
    case InputDevice::Key::LEFT:                return VK_LEFT;
    case InputDevice::Key::UP:                  return VK_UP;
    case InputDevice::Key::RIGHT:               return VK_RIGHT;
    case InputDevice::Key::DOWN:                return VK_DOWN;
    default:                                    return 0x07; // undefined
        // clang-format on
    }
}

// we cannot just use WS_POPUP style
// WS_THICKFRAME: without this the window cannot be resized and so aero snap, de-maximizing and minimizing won't work
// WS_SYSMENU: enables the context menu with the move, close, maximize, minize... commands (shift + right-click on the task bar item)
// WS_CAPTION: enables aero minimize animation/transition
// WS_MAXIMIZEBOX, WS_MINIMIZEBOX: enable minimize/maximize
enum class Style : DWORD {
    windowed = WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
    aero_borderless = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
    basic_borderless = WS_POPUP | WS_THICKFRAME | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX
};

auto maximized(HWND hwnd) -> bool {
    WINDOWPLACEMENT placement;
    if (!::GetWindowPlacement(hwnd, &placement)) {
        return false;
    }

    return placement.showCmd == SW_MAXIMIZE;
}

/* Adjust client rect to not spill over monitor edges when maximized.
* rect(in/out): in: proposed window rect, out: calculated client rect
* Does nothing if the window is not maximized.
*/
auto adjust_maximized_client_rect(HWND window, RECT& rect) -> void {
    if (!maximized(window)) {
        return;
    }

    auto monitor = ::MonitorFromWindow(window, MONITOR_DEFAULTTONULL);
    if (!monitor) {
        return;
    }

    MONITORINFO monitor_info{};
    monitor_info.cbSize = sizeof(monitor_info);
    if (!::GetMonitorInfoW(monitor, &monitor_info)) {
        return;
    }

    // when maximized, make the client area fill just the monitor (without task bar) rect,
    // not the whole window rect which extends beyond the monitor.
    rect = monitor_info.rcWork;
}

auto window_class(WNDPROC wndproc, std::wstring_view wtitle) -> const wchar_t* {
    static const wchar_t* window_class_name = [&] {
        WNDCLASSEXW wcx{};
        wcx.cbSize = sizeof(wcx);
        wcx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wcx.cbClsExtra = 0;
        wcx.cbWndExtra = 24;
        wcx.hInstance = nullptr;
        wcx.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
        wcx.hIconSm = wcx.hIcon;
        wcx.lpfnWndProc = wndproc;
        wcx.lpszClassName = wtitle.data();
        wcx.hbrBackground = reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));
        wcx.hCursor = ::LoadCursorW(nullptr, IDC_ARROW);
        const ATOM result = ::RegisterClassExW(&wcx);
        if (!result) {
            throw utils::last_error("failed to register window class");
        }
        return wcx.lpszClassName;
    }();
    return window_class_name;
}

auto composition_enabled() -> bool {
    BOOL composition_enabled = FALSE;
    bool success = ::DwmIsCompositionEnabled(&composition_enabled) == S_OK;
    return composition_enabled && success;
}

auto select_borderless_style() -> Style {
    return composition_enabled() ? Style::aero_borderless : Style::basic_borderless;
}

auto set_shadow(HWND handle, bool enabled) -> void {
    if (composition_enabled()) {
        static const MARGINS shadow_state[2]{ { 0, 0, 0, 0 }, { 1, 1, 1, 1 } };
        ::DwmExtendFrameIntoClientArea(handle, &shadow_state[enabled]);
    }
}

auto create_window(WNDPROC wndProc, void* userdata, std::wstring_view wtitle) -> Display::unique_handle {
    //WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    WNDCLASSEXW wcx{};
    wcx.cbSize = sizeof(wcx);
    wcx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 24;
    wcx.hInstance = nullptr;
    wcx.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
    wcx.hIconSm = wcx.hIcon;
    wcx.lpfnWndProc = wndProc;
    wcx.lpszClassName = wtitle.data();
    wcx.hbrBackground = NULL; //reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1); //::GetStockObject(BLACK_BRUSH));
    wcx.hCursor = ::LoadCursorW(nullptr, IDC_ARROW);
    const ATOM result = ::RegisterClassExW(&wcx);
    if (!result) {
        throw utils::last_error("failed to register window class");
    }

    auto handle = CreateWindowExW(
      0, wcx.lpszClassName, wtitle.data(),
      static_cast<DWORD>(Style::aero_borderless), CW_USEDEFAULT, CW_USEDEFAULT,
      1280, 720, nullptr, nullptr, nullptr, userdata);
    if (!handle) {
        throw utils::last_error("failed to create window");
    }
    return Display::unique_handle{ handle };
}
} // namespace

Display::Display(std::string_view title, HINSTANCE hInstance, InputDevice& inputDevice)
    : inputDevice_{ inputDevice }
    , hInstance_{ hInstance }
    // TODO: convert
    , wtitle_{ title.begin(), title.end() }
    , hWnd_{ create_window(&Display::wndProc_, this, std::wstring{ title.begin(), title.end() }) } {

    setBorderless(borderless_);
    setBorderlessShadow(borderlessShadow_);
    ::ShowWindow(hWnd_.get(), SW_SHOW);

    // Determine the resolution of the clients desktop screen.
    auto screenWidth = GetSystemMetrics(SM_CXSCREEN);
    auto screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // If windowed then set it to 800x800 resolution.
    int width = 800;
    int height = 800;

    // Place the window in the middle of the screen.
    auto posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
    auto posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

    RECT windowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    //auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME; // WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP
    // Create the window with the screen settings and get the handle to it.
    //auto handle hWnd_ = CreateWindowEx(WS_EX_APPWINDOW, wtitle.c_str(), wtitle.c_str(),
    //  dwStyle,
    //  posX, posY,
    //  windowRect.right - windowRect.left,
    //  windowRect.bottom - windowRect.top,
    //  nullptr, nullptr, hInstance, this);

    // SetWindowPos(h_wnd_,
    //   HWND_TOP,
    //   posX, posY,
    //   windowRect.right - windowRect.left,
    //   windowRect.bottom - windowRect.top,
    //   SWP_NOSIZE | SWP_NOZORDER);

    //SetWindowTheme(h_wnd_, L" ", L" ");

    //ShowWindow(hWnd_.get(), SW_SHOW);
    SetForegroundWindow(hWnd_.get());
    SetFocus(hWnd_.get());

    ShowCursor(true);

    //inputDevice_.set_key_state_getter([](input_device::key key) -> bool {
    //    return GetKeyState(from_input_device_key(key)) & 0x8000;
    //});
    //input_device_.set_mouse_position_getter([h_wnd{ h_wnd_ }] {
    //    POINT point;
    //    //POINT* pp = &point;
    //    bool res = GetCursorPos(&point);
    //    //MapWindowPoints(NULL, h_wnd, &pp, 1);
    //    res = ScreenToClient(h_wnd, &point);
    //    return std::tuple<int, int>{ point.x, point.y };
    //});

    if (!createDevice_(hWnd_.get())) {
        cleanupDevice_();
        ::UnregisterClass(wtitle_.c_str(), hInstance);
        return;
    }
}

auto Display::runLoop() const -> void {
    MSG msg_;
    bool is_exit_requested_{ false };
    // Loop until there is a quit message from the window or the user.
    while (!is_exit_requested_) {
        // Handle the windows messages.
        while (PeekMessage(&msg_, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg_);
            DispatchMessage(&msg_);
        }

        // If windows signals to end the application then exit out.
        if (msg_.message == WM_QUIT) {
            is_exit_requested_ = true;
        }

        if (frameDelegate_) {
            frameDelegate_(0.f);
        }

        // utils::debug_write::info("mouse {} {}\n", std::get<0>(input_device_.get_mouse_position().unwrap()), std::get<1>(input_device_.get_mouse_position().unwrap()));
        //auto id = std::this_thread::get_id();
        //std::cout << "Main id: " << id << "\n";
    }
}

namespace {
WPARAM MapLeftRightKeys(WPARAM vk, LPARAM lParam) {
    WPARAM new_vk = vk;
    UINT scancode = (lParam & 0x00ff0000) >> 16;
    int extended = (lParam & 0x01000000) != 0;

    switch (vk) {
    case VK_SHIFT:
        new_vk = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
        break;
    case VK_CONTROL:
        new_vk = extended ? VK_RCONTROL : VK_LCONTROL;
        break;
    case VK_MENU:
        new_vk = extended ? VK_RMENU : VK_LMENU;
        break;
    default:
        // not a key we map from generic to left/right specialized
        //  just return it.
        new_vk = vk;
        break;
    }

    return new_vk;
}
} // namespace

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

auto CALLBACK Display::wndProc_(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) -> LRESULT {
    //auto id = std::this_thread::get_id();
    //WriteInfo() << "WndProc id: " << id << "\n";

    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMessage, wParam, lParam)) {
        return true;
    }

    if (uMessage == WM_NCCREATE) {
        auto userdata = reinterpret_cast<CREATESTRUCTW*>(lParam)->lpCreateParams;
        // store window instance pointer in window user data
        ::SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(userdata));
    }

    if (auto self_ptr = reinterpret_cast<Display*>(::GetWindowLongPtrW(hWnd, GWLP_USERDATA))) {
        auto& self = *self_ptr;

        switch (uMessage) {

        case WM_NCCALCSIZE: {
            if (wParam == TRUE && self.borderless_) {
                auto& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
                adjust_maximized_client_rect(hWnd, params.rgrc[0]);
                return 0;
            }
            break;
        }
        case WM_NCHITTEST: {
            // When we have no border or title bar, we need to perform our
            // own hit testing to allow resizing and moving.
            if (self.borderless_) {
                return self.hitTest_(POINT{
                  GET_X_LPARAM(lParam),
                  GET_Y_LPARAM(lParam) });
            }
            break;
        }
        case WM_NCACTIVATE: {
            if (!composition_enabled()) {
                // Prevents window frame reappearing on window activation
                // in "basic" theme, where no aero shadow is present.
                return 1;
            }
            break;
        }

        //case WM_NCCREATE: {
        //    SetLastError(0);
        //    SetWindowLongPtr(hWnd, DWLP_USER, (LONG_PTR)(((CREATESTRUCT*)lParam)->lpCreateParams));
        //    utils::debug_write::info("SetWindowLongPtr: {}\n", GetLastError());
        //    return TRUE;
        //}
        // Check if the window is being destroyed.
        case WM_CLOSE: {
            ::DestroyWindow(hWnd);
            return 0;
        }

        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }

        case WM_SIZE: {
            if (self.device != NULL && wParam != SIZE_MINIMIZED) {
                self.cleanupRenderTarget_();
                self.swapchain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
                self.createRenderTarget_();
            }
            //auto self = (Display*)GetWindowLongPtr(hWnd, DWLP_USER);
            self.onResized_({ LOWORD(lParam), HIWORD(lParam) });
            utils::debug_write::info("Width {} Hight {}\n", LOWORD(lParam), HIWORD(lParam));
            return 0;
        }

        case WM_DPICHANGED: {
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports) {
                //const int dpi = HIWORD(wParam);
                //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
                const RECT* suggested_rect = (RECT*)lParam;
                ::SetWindowPos(hWnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
            }
            break;
        }

        case WM_SYSCOMMAND: {
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;
        }

        //case WM_WINDOWPOSCHANGED: {
        //    return 0;
        //}

        // Check if a key has been pressed on the keyboard.
        case WM_KEYDOWN: {
            wParam = MapLeftRightKeys(wParam, lParam);
            switch (wParam) {
            case VK_F11: {
                self.setFullscreenWindowed(!self.fullscreenWindowed_);
                return 0;
            }
            default:
                break;
            }
            // If a key is pressed send it to the input object so it can record that state.
            if (toInputDeviceKey(wParam) != InputDevice::Key::NONE) {
                //auto self = (Display*)GetWindowLongPtr(hWnd, DWLP_USER);
                self.inputDevice_.onKeyPressed(toInputDeviceKey(wParam));
            }
            utils::debug_write::info("key: {}\n", wParam);

            if (static_cast<unsigned int>(wParam) == 27) {
                PostQuitMessage(0);
            }
            return 0;
        }

        // Check if a key has been released on the keyboard.
        case WM_KEYUP: {
            wParam = MapLeftRightKeys(wParam, lParam);
            if (toInputDeviceKey(wParam) != InputDevice::Key::NONE) {
                //auto self = (Display*)GetWindowLongPtr(hWnd, DWLP_USER);
                self.inputDevice_.onKeyReleased(toInputDeviceKey(wParam));
            }
            // If a key is released then send it to the input object so it can unset the state for that key.
            return 0;
        }

        case WM_MOUSEMOVE: {
            auto xPos = GET_X_LPARAM(lParam);
            auto yPos = GET_Y_LPARAM(lParam);
            //auto self = (Display*)GetWindowLongPtr(hWnd, DWLP_USER);
            self.inputDevice_.onMouseMoved(InputDevice::MousePos{ .x = xPos, .y = yPos });
            return 0;
        }

        case WM_LBUTTONDOWN: {
            //auto self = (Display*)GetWindowLongPtr(hWnd, DWLP_USER);
            self.inputDevice_.onKeyPressed(InputDevice::Key::MOUSE_BUTTON_LEFT);
            return 0;
        }
        case WM_LBUTTONUP: {
            //auto self = (Display*)GetWindowLongPtr(hWnd, DWLP_USER);
            self.inputDevice_.onKeyReleased(InputDevice::Key::MOUSE_BUTTON_LEFT);
            return 0;
        }
        case WM_RBUTTONDOWN: {
            //auto self = (Display*)GetWindowLongPtr(hWnd, DWLP_USER);
            self.inputDevice_.onKeyPressed(InputDevice::Key::MOUSE_BUTTON_RIGHT);
            return 0;
        }
        case WM_RBUTTONUP: {
            //auto self = (Display*)GetWindowLongPtr(hWnd, DWLP_USER);
            self.inputDevice_.onKeyReleased(InputDevice::Key::MOUSE_BUTTON_RIGHT);
            return 0;
        }
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
            break;
            // All other messages pass to the message handler in the system class.
            //default: {
            //    return DefWindowProc(hWnd, uMessage, wParam, lParam);
            //}
        }
    }

    return ::DefWindowProcW(hWnd, uMessage, wParam, lParam);
}

auto Display::size() const -> Display::DisplaySize {
    RECT rect;
    if (!GetWindowRect(hWnd_.get(), &rect)) {
        throw std::runtime_error("con not get window rect");
    }
    return DisplaySize{
        .width = (rect.right - rect.left),
        .height = (rect.bottom - rect.top)
    };
}

auto Display::show() -> void {
    throw std::runtime_error("not implemented");
}

auto Display::setTitle(std::string title) -> void {
    throw std::runtime_error("not implemented");
}

auto Display::hWnd() const -> HWND {
    if (!hWnd_) {
        throw std::runtime_error("window was not created");
    }
    return hWnd_.get();
}

auto Display::onResized_(DisplaySize size) -> void {
    resizedSigh_.publish(size);
}

auto Display::setBorderless(bool enabled) -> void {
    Style new_style = (enabled) ? select_borderless_style() : Style::windowed;
    Style old_style = static_cast<Style>(::GetWindowLongPtrW(hWnd_.get(), GWL_STYLE));

    if (new_style != old_style) {
        borderless_ = enabled;

        ::SetWindowLongPtrW(hWnd_.get(), GWL_STYLE, static_cast<LONG>(new_style));

        // when switching between borderless and windowed, restore appropriate shadow state
        set_shadow(hWnd_.get(), borderlessShadow_ && (new_style != Style::windowed));

        // redraw frame
        ::SetWindowPos(hWnd_.get(), nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
        ::ShowWindow(hWnd_.get(), SW_SHOW);
    }
}

auto Display::setBorderlessShadow(bool enabled) -> void {
    if (borderless_) {
        borderlessShadow_ = enabled;
        set_shadow(hWnd_.get(), enabled);
    }
}

auto Display::hitTest_(POINT cursor) const -> LRESULT {
    // identify borders and corners to allow resizing the window.
    // Note: On Windows 10, windows behave differently and
    // allow resizing outside the visible window frame.
    // This implementation does not replicate that behavior.
    const POINT border{
        ::GetSystemMetrics(SM_CXFRAME), // + ::GetSystemMetrics(SM_CXPADDEDBORDER),
        ::GetSystemMetrics(SM_CYFRAME) // + ::GetSystemMetrics(SM_CXPADDEDBORDER)
    };
    RECT window;
    if (!::GetWindowRect(hWnd_.get(), &window)) {
        return HTNOWHERE;
    }

    const auto drag = noDrag_ ? HTCLIENT : borderlessDrag_ && drag_ ? HTCAPTION
                                                                    : HTCLIENT;

    enum region_mask {
        client = 0b0000,
        left = 0b0001,
        right = 0b0010,
        top = 0b0100,
        bottom = 0b1000,
    };

    const auto result
      = left * (cursor.x < (window.left + border.x))
      | right * (cursor.x >= (window.right - border.x))
      | top * (cursor.y < (window.top + border.y))
      | bottom * (cursor.y >= (window.bottom - border.y));

    switch (result) {
    case left:
        return borderlessResize_ ? HTLEFT : drag;
    case right:
        return borderlessResize_ ? HTRIGHT : drag;
    case top:
        return borderlessResize_ ? HTTOP : drag;
    case bottom:
        return borderlessResize_ ? HTBOTTOM : drag;
    case top | left:
        return borderlessResize_ ? HTTOPLEFT : drag;
    case top | right:
        return borderlessResize_ ? HTTOPRIGHT : drag;
    case bottom | left:
        return borderlessResize_ ? HTBOTTOMLEFT : drag;
    case bottom | right:
        return borderlessResize_ ? HTBOTTOMRIGHT : drag;
    case client:
        return drag;
    default:
        return HTNOWHERE;
    }
}

auto Display::setDrag(bool enabled) -> void {
    drag_ = enabled;
}

auto Display::createRenderTarget_() -> void {
    ID3D11Texture2D* pBackBuffer;
    swapchain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    device->CreateRenderTargetView(pBackBuffer, NULL, &renderTargetView);
    pBackBuffer->Release();
}

auto Display::cleanupRenderTarget_() -> void {
    if (renderTargetView) {
        renderTargetView->Release();
        //g_mainRenderTargetView = NULL;
    }
}

auto Display::createDevice_(HWND hWnd) -> bool {
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, swapchain.GetAddressOf(), device.GetAddressOf(), &featureLevel, deviceContext.GetAddressOf()) != S_OK)
        return false;

    createRenderTarget_();
    return true;
}

auto Display::cleanupDevice_() -> void {
    cleanupRenderTarget_();
    if (swapchain) {
        swapchain->Release();
    }
    if (deviceContext) {
        deviceContext->Release();
    }
    if (device) {
        device->Release();
    }
}

auto Display::setFullscreenWindowed(bool enabled) -> void {
    if (enabled == fullscreenWindowed_) {
        return;
    }

    if (enabled) {
        ::GetWindowRect(hWnd_.get(), &savedRect_);
        setBorderless(true);
        setBorderlessShadow(false);
        borderlessResize_ = false;
        noDrag_ = true;
        auto screenWidth = GetSystemMetrics(SM_CXSCREEN);
        auto screenHeight = GetSystemMetrics(SM_CYSCREEN);
        ::SetWindowPos(hWnd_.get(), HWND_NOTOPMOST, 0, 0, screenWidth, screenHeight, 0);
    } else {
        setBorderless(false);
        ::SetWindowPos(hWnd_.get(), HWND_NOTOPMOST, savedRect_.left, savedRect_.top, savedRect_.right, savedRect_.bottom, 0);
        setBorderlessShadow(true);
        borderlessResize_ = true;
        noDrag_ = false;
    }

    fullscreenWindowed_ = enabled;
}