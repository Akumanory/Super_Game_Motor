#include <gtest/gtest.h>

#include <motor/async/resource.hpp>

using namespace motor;

TEST(SharedResource, Simple) {
    struct A {
        int a;
        long long b;
        double c;
        ~A() {
            std::cout << "destructed" << std::endl;
        }
    };
    {
        async::shared_resource<A> sr{ 4, 5, 6.7 };
        async::shared_resource<A> sr2{ A{ 8, 9, 10.11 } };
        sr.read([](A const& a) {
            EXPECT_EQ(a.a, 4);
            EXPECT_EQ(a.b, 5);
            EXPECT_DOUBLE_EQ(a.c, 6.7);
        });
        sr2.read([](A const& a) {
            EXPECT_EQ(a.a, 8);
            EXPECT_EQ(a.b, 9);
            EXPECT_DOUBLE_EQ(a.c, 10.11);
        });
        sr2 = sr;
        sr2.read([](A const& a) {
            EXPECT_EQ(a.a, 4);
            EXPECT_EQ(a.b, 5);
            EXPECT_DOUBLE_EQ(a.c, 6.7);
        });
        sr.write([](A& a) {
            a.a = 1;
            a.b = 2;
            a.c = 3.3;
        });
        sr.read([](A const& a) {
            EXPECT_EQ(a.a, 1);
            EXPECT_EQ(a.b, 2);
            EXPECT_DOUBLE_EQ(a.c, 3.3);
        });
        sr.write([](A const& a) {
            EXPECT_EQ(a.a, 1);
            EXPECT_EQ(a.b, 2);
            EXPECT_DOUBLE_EQ(a.c, 3.3);
        });
        EXPECT_TRUE(sr);
        sr2 = std::move(sr);
        EXPECT_FALSE(sr);
        sr2.read([](A const& a) {
            EXPECT_EQ(a.a, 1);
            EXPECT_EQ(a.b, 2);
            EXPECT_DOUBLE_EQ(a.c, 3.3);
        });
    }
    {
        async::shared_resource<A> sr{ 1, 2, 3.4 };
        auto f = [](async::shared_resource<A> const& sr) {
            if (sr) {
                sr.read([](A const& a) {
                    EXPECT_EQ(a.a, 1);
                    std::cout << "f()" << std::endl;
                });
            }
        };
        f(sr);
    }
    {
        async::shared_resource<A> sr, sr2, sr3;
        sr.init(1, 2, 3.4);
        sr.destroy();
        sr.init(2, 3, 4.5);
        sr.read([&sr, &sr2, &sr3](A const& a) {
            EXPECT_EQ(a.a, 2);
            sr2 = sr;
            sr2.read([&sr, &sr3](A const& a) {
                EXPECT_EQ(a.a, 2);
                sr3 = sr;
                sr3.write([](A const& a) {
                    EXPECT_EQ(a.a, 2);
                });
            });
        });
    }
}

TEST(SharedResource, TrivialCopy) {
    struct Trivial {
        int a;
        long long b;
        double c;
    };
    static_assert(std::is_trivial_v<Trivial>);
    {
        async::shared_resource<Trivial> sr{ 4, 5, 6.7 };
        async::shared_resource<Trivial> sr2{ Trivial{ 8, 9, 10.11 } };
    }
}
