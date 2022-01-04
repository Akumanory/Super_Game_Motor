#include <motor/converters.hpp>

#include <cwchar>

namespace motor {
namespace converters {

auto w_to_mb(std::wstring_view in) -> std::string {
    std::string ret;
    ret.reserve(in.size() * 3);
    std::mbstate_t state{};
    for (wchar_t wc : in) {
        std::string mb(MB_CUR_MAX, '\0');
        std::size_t rc = std::wcrtomb(&mb[0], wc, &state);
        for (auto c : std::string_view{ mb.c_str(), rc }) {
            ret.push_back(c);
        }
    }
    ret.shrink_to_fit();
    return ret;
}

auto mb_to_w(std::string_view in) -> std::wstring {
    std::wstring ret;
    ret.reserve(in.size());
    std::mbstate_t state{};
    const char* ptr = &in[0];
    const char* end = ptr + in.size();
    int len;
    wchar_t wc;
    while ((len = std::mbrtowc(&wc, ptr, end - ptr, &state)) > 0) {
        ret.push_back(wc);
        ptr += len;
    }
    ret.shrink_to_fit();
    return ret;
}

} // namespace converters
} // namespace motor