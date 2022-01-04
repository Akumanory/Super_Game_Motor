#include <motor/converters.hpp>

#include <cuchar>
#include <stdexcept>

namespace motor {
namespace converters {

auto u8_to_mb(std::u8string_view in) -> std::string {
    return { in.begin(), in.end() };
}

auto u16_to_mb(std::u16string_view in) -> std::string {
    std::string ret;
    ret.reserve(in.size());
    std::mbstate_t state{};
    char out[MB_LEN_MAX]{};
    for (char16_t c : in) {
        std::size_t rc = std::c16rtomb(out, c, &state);
        if (rc == (std::size_t)-1) {
            throw std::runtime_error("Failed to convert from mb to u16");
        }
        for (unsigned char c8 : std::string_view{ out, rc }) {
            ret.push_back(c8);
        }
    }
    ret.shrink_to_fit();
    return ret;
}

auto u32_to_mb(std::u32string_view in) -> std::string {
    std::string ret;
    ret.reserve(in.size());
    std::mbstate_t state{};
    char out[MB_LEN_MAX]{};
    for (char32_t c : in) {
        std::size_t rc = std::c32rtomb(out, c, &state);
        if (rc == (std::size_t)-1) {
            throw std::runtime_error("Failed to convert from mb to u32");
        }
        for (unsigned char c8 : std::string_view{ out, rc }) {
            ret.push_back(c8);
        }
    }
    ret.shrink_to_fit();
    return ret;
}

auto mb_to_u8(std::string_view in) -> std::u8string {
    return { in.begin(), in.end() };
}

auto mb_to_u16(std::string_view in) -> std::u16string {
    std::u16string ret;
    ret.reserve(in.size() * 2);
    std::mbstate_t state{};
    char16_t c16;
    const char *ptr = &in[0], *end = &in[0] + in.size();

    while (std::size_t rc = std::mbrtoc16(&c16, ptr, end - ptr + 1, &state)) {
        ret.push_back(c16);
        if (rc == (std::size_t)-3) {
            // std::cout << "earlier surrogate pair\n";
        } else if (rc == (std::size_t)-2) {
            throw std::runtime_error("Failed to convert from u16 to mb");
        } else if (rc == (std::size_t)-1) {
            throw std::runtime_error("Failed to convert from u16 to mb");
        } else {
            ptr += rc;
        }
    }

    ret.shrink_to_fit();
    return ret;
}

auto mb_to_u32(std::string_view in) -> std::u32string {
    std::u32string ret;
    ret.reserve(in.size() * 4);
    std::mbstate_t state{};
    char32_t c32;
    const char *ptr = &in[0], *end = &in[0] + in.size() + 1;

    while (std::size_t rc = std::mbrtoc32(&c32, ptr, end - ptr, &state)) {
        ret.push_back(c32);
        if (rc == (std::size_t)-3) {
            throw std::runtime_error("Failed to convert from u32 to mb");
        } else if (rc == (std::size_t)-2) {
            throw std::runtime_error("Failed to convert from u32 to mb");
        } else if (rc == (std::size_t)-1) {
            throw std::runtime_error("Failed to convert from u32 to mb");
        } else {
            ptr += rc;
        }
    }

    ret.shrink_to_fit();
    return ret;
}

} // namespace converters
} // namespace motor