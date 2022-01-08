#include <motor/core_system/input_device.hpp>

using namespace motor;

auto InputDevice::onKeyPressed(Key key) -> void {
    keys_[static_cast<std::underlying_type_t<Key>>(key)] = true;
    eventSigh_.publish(Event{
      .mousePos = { 0, 0 },
      .key = key,
      .isKey = true,
      .isPressed = true,
      .isReleased = false,
      .isMouseMoved = false,
    });
}

auto InputDevice::onKeyReleased(Key key) -> void {
    keys_[static_cast<std::underlying_type_t<Key>>(key)] = false;
    eventSigh_.publish(Event{
      .mousePos = { 0, 0 },
      .key = key,
      .isKey = true,
      .isPressed = false,
      .isReleased = true,
      .isMouseMoved = false,
    });
}

auto InputDevice::onMouseMoved(MousePos mousePos) -> void {
    mousePos_ = mousePos;
    eventSigh_.publish(Event{
      .mousePos = mousePos_,
      .key = Key::NONE,
      .isKey = false,
      .isPressed = false,
      .isReleased = false,
      .isMouseMoved = true,
    });
}

constexpr auto InputDevice::keyToStr(Key key) -> std::string_view {
    switch (key) {
        // clang-format off
    case InputDevice::Key::NONE:                return "NONE";
    case InputDevice::Key::MOUSE_BUTTON_LEFT:   return "MOUSE_BUTTON_LEFT";
    case InputDevice::Key::MOUSE_BUTTON_RIGHT:  return "MOUSE_BUTTON_RIGHT";
    case InputDevice::Key::A:                   return "A";
    case InputDevice::Key::D:                   return "D";
    case InputDevice::Key::S:                   return "S";
    case InputDevice::Key::W:                   return "W";
    case InputDevice::Key::E:                   return "E";
    case InputDevice::Key::Q:                   return "Q";
    case InputDevice::Key::SPACEBAR:            return "SPACEBAR";
    case InputDevice::Key::ESCAPE:              return "ESCAPE";
    case InputDevice::Key::ENTER:               return "ENTER";
    case InputDevice::Key::LEFT_SHIFT:          return "LEFT_SHIFT";
    case InputDevice::Key::RIGHT_SHIFT:         return "RIGHT_SHIFT";
    case InputDevice::Key::LEFT_CTRL:           return "LEFT_CTRL";
    case InputDevice::Key::RIGHT_CTRL:          return "RIGHT_CTRL";
    case InputDevice::Key::LEFT:                return "LEFT";
    case InputDevice::Key::UP:                  return "UP";
    case InputDevice::Key::RIGHT:               return "RIGHT";
    case InputDevice::Key::DOWN:                return "DOWN";
    case InputDevice::Key::TILDA:               return "TILDA";
    default:                                    return "NONE"; // undefined
        // clang-format on
    }
}

constexpr auto hash(std::string_view str) {
    std::uint64_t result{ 0 };
    for (auto c : str) {
        result = static_cast<std::uint64_t>(c) + result * 257;
    }
    return result;
}

constexpr auto InputDevice::strToKey(std::string_view str) -> Key {
    auto keyHash = hash(str);
    switch (keyHash) {
        // clang-format off
    case hash("NONE"):                  return InputDevice::Key::NONE;
    case hash("MOUSE_BUTTON_LEFT"):     return InputDevice::Key::MOUSE_BUTTON_LEFT;
    case hash("MOUSE_BUTTON_RIGHT"):    return InputDevice::Key::MOUSE_BUTTON_RIGHT;
    case hash("A"):                     return InputDevice::Key::A;
    case hash("D"):                     return InputDevice::Key::D;
    case hash("E"):                     return InputDevice::Key::E;
    case hash("Q"):                     return InputDevice::Key::Q;
    case hash("S"):                     return InputDevice::Key::S;
    case hash("W"):                     return InputDevice::Key::W;
    case hash("SPACEBAR"):              return InputDevice::Key::SPACEBAR;
    case hash("ESCAPE"):                return InputDevice::Key::ESCAPE;
    case hash("ENTER"):                 return InputDevice::Key::ENTER;
    case hash("LEFT_SHIFT"):            return InputDevice::Key::LEFT_SHIFT;
    case hash("RIGHT_SHIFT"):           return InputDevice::Key::RIGHT_SHIFT;
    case hash("LEFT_CTRL"):             return InputDevice::Key::LEFT_CTRL;
    case hash("RIGHT_CTRL"):            return InputDevice::Key::RIGHT_CTRL;
    case hash("LEFT"):                  return InputDevice::Key::LEFT;
    case hash("UP"):                    return InputDevice::Key::UP;
    case hash("RIGHT"):                 return InputDevice::Key::RIGHT;
    case hash("DOWN"):                  return InputDevice::Key::DOWN;
    case hash("TILDA"):                 return InputDevice::Key::TILDA;
    default:                            return InputDevice::Key::NONE;
        // clang-format on
    }
}