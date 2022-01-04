#pragma once

#include <string_view>

namespace motor {
namespace converters {

auto u8_to_mb(std::u8string_view) -> std::string;
auto u16_to_mb(std::u16string_view) -> std::string;
auto u32_to_mb(std::u32string_view) -> std::string;
auto w_to_mb(std::wstring_view) -> std::string;

auto mb_to_u8(std::string_view) -> std::u8string;
auto mb_to_u16(std::string_view) -> std::u16string;
auto mb_to_u32(std::string_view) -> std::u32string;
auto mb_to_w(std::string_view) -> std::wstring;

auto w_to_u8(std::wstring_view) -> std::u8string;
auto u8_to_w(std::u8string_view) -> std::wstring;

} // namespace converters
} // namespace motor