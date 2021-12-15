#pragma once

#include <entt/entt.hpp>

namespace motor {

class InputDevice {
public:
    struct MousePos {
        int x;
        int y;
    };

    enum class Key : std::uint8_t {
        NONE,
        MOUSE_BUTTON_LEFT,
        MOUSE_BUTTON_RIGHT,
        W,
        A,
        S,
        D,
        Q,
        E,
        SPACEBAR,
        ESCAPE,
        ENTER,
        LEFT_SHIFT,
        RIGHT_SHIFT,
        LEFT_CTRL,
        RIGHT_CTRL,
        LEFT,
        UP,
        RIGHT,
        DOWN,
        TILDA,
        _COUNT,
    };

    struct Event {
        MousePos mousePos;
        Key key;
        bool isKey;
        bool isPressed;
        bool isReleased;
        bool isMouseMoved;
    };

public:
    InputDevice() = default;
    InputDevice(InputDevice const&) = delete;
    InputDevice(InputDevice&&) = default;
    InputDevice& operator=(InputDevice const&) = delete;
    InputDevice& operator=(InputDevice&&) = default;
    ~InputDevice() = default;

public:
    auto isGamepadEnabled() -> bool { return false; }
    auto mouseCoord() -> MousePos { return mousePos_; }
    auto isKeyPressed(Key key) -> bool { return keys_[static_cast<std::underlying_type_t<Key>>(key)]; }
    auto eventSink() -> entt::sink<void(Event)>& { return eventSink_; }

    auto onKeyPressed(Key key) -> void;
    auto onKeyReleased(Key key) -> void;
    auto onMouseMoved(MousePos pos) -> void;

    static constexpr auto keyToStr(Key key) -> std::string_view;
    static constexpr auto strToKey(std::string_view) -> Key;

private:
    bool keys_[256];
    MousePos mousePos_;

    entt::sigh<void(Event)> eventSigh_;
    entt::sink<void(Event)> eventSink_{ eventSigh_ };
};

}; // namespace motor