#include <motor/utils.hpp>

#include <iostream>

namespace motor {
namespace utils {

void debug_write::info_(std::string_view const s) {
    std::cout << s;
    OutputDebugStringA(s.data());
}

void debug_write::error_(std::string_view const s) {
    std::cerr << s;
    OutputDebugStringA(s.data());
}

void enable_console() {
    AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
}

auto last_error(const std::string& message) -> std::system_error {
    return std::system_error(
      std::error_code(::GetLastError(), std::system_category()),
      message);
}

} // namespace utils
} // namespace motor
