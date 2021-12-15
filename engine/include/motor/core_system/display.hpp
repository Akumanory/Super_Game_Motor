#pragma once

#include <motor/core_system/input_device.hpp>

#include <entt/entt.hpp>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef min
#undef max
#include <d3d11.h>
#include <wrl.h>

namespace motor {

class Display {
public:
    struct DisplaySize {
        int width;
        int height;
    };

    struct hwnd_deleter {
        using pointer = HWND;
        auto operator()(HWND handle) const -> void {
            ::DestroyWindow(handle);
        }
    };
    using unique_handle = std::unique_ptr<HWND, hwnd_deleter>;

public:
    Display(std::string_view title, HINSTANCE hInstance, InputDevice& input_device);
    Display(Display const&) = delete;
    Display(Display&&) = default;
    Display& operator=(Display const&) = delete;
    Display& operator=(Display&&) = default;
    ~Display() = default;

public:
    auto runLoop() const -> void;
    auto frameDelegate() -> entt::delegate<void(float)> { return frameDelegate_; }
    auto size() const -> DisplaySize;
    auto show() -> void;
    auto setTitle(std::string title) -> void;
    auto hWnd() const -> HWND;
    auto resizedSink() -> entt::sink<void(DisplaySize)>& { return resizedSink_; }
    auto setDrag(bool enabled) -> void;
    auto setBorderless(bool enabled) -> void;
    auto setBorderlessShadow(bool enabled) -> void;
    auto setFullscreenWindowed(bool enabled) -> void;

private:
    static auto CALLBACK wndProc_(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto onResized_(DisplaySize) -> void;
    auto hitTest_(POINT cursor) const -> LRESULT;
    auto createRenderTarget_() -> void;
    auto cleanupRenderTarget_() -> void;
    auto createDevice_(HWND) -> bool;
    auto cleanupDevice_() -> void;

public:
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

private:
    entt::sigh<void(DisplaySize)> resizedSigh_;
    entt::sink<void(DisplaySize)> resizedSink_{ resizedSigh_ };
    entt::delegate<void(float)> frameDelegate_;
    InputDevice& inputDevice_;
    HINSTANCE hInstance_;
    unique_handle hWnd_;
    std::wstring wtitle_;
    bool borderless_ = false; // is the window currently borderless
    bool borderlessResize_ = true; // should the window allow resizing by dragging the borders while borderless
    bool borderlessDrag_ = true; // should the window allow moving by dragging the client area
    bool borderlessShadow_ = true; // should the window display a native aero shadow while borderless
    bool fullscreenWindowed_ = false;
    bool noDrag_ = false;
    bool drag_ = false;
    RECT savedRect_;
};

} // namespace motor