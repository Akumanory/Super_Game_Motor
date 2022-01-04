#include <motor/converters.hpp>

namespace motor {
namespace converters {

auto w_to_u8(std::wstring_view in) -> std::u8string {
    return mb_to_u8(w_to_mb(in));
}

auto u8_to_w(std::u8string_view in) -> std::wstring {
    return mb_to_w(u8_to_mb(in));
}

} // namespace converters
} // namespace motor