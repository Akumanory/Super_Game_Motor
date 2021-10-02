#include <gtest/gtest.h>

#include <motor/utils.hpp>

using namespace motor;
using namespace utils;

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

TEST(DebugTest, ThrowIfFailed) {
    try {
        throw_if_failed(E_FAIL);
    } catch (com_exception& e) {
        utils::debug_write::error(e.what());
    }
}
