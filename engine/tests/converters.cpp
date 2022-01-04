#include <gtest/gtest.h>

#include <motor/converters.hpp>

using namespace motor::converters;

TEST(Converters, Simple) {
    std::string s = "\n\n\x7a\xc3\x9f\xe6\xb0\xb4\xf0\x9d\x84\x8b\n\r\n\r\n\x7a\xc3\x9f\xe6\xb0\xb4\xf0\x9d\x84\x8b\n";

    for (int i = 0; i < 12; ++i) {
        s += s;
    }

    auto ws = mb_to_w(s);
    auto su16 = mb_to_u16(s);
    ASSERT_STREQ((const char*)ws.c_str(), (const char*)su16.c_str());

    auto su32 = mb_to_u32(s);
    auto s2 = u32_to_mb(su32);
    ASSERT_STREQ((const char*)s.c_str(), (const char*)s2.c_str());

    auto su8 = w_to_u8(ws);
    auto s3 = u8_to_mb(su8);

    ASSERT_STREQ((const char*)s.c_str(), (const char*)s3.c_str());

    auto su8_2 = mb_to_u8(s);
    ASSERT_STREQ((const char*)su8.c_str(), (const char*)su8_2.c_str());

    auto ws_2 = u8_to_w(su8_2);
    auto s4 = w_to_mb(ws_2);
    ASSERT_STREQ((const char*)s4.c_str(), (const char*)s.c_str());

    auto s5 = u16_to_mb(su16);
    ASSERT_STREQ((const char*)s5.c_str(), (const char*)s.c_str());
}
