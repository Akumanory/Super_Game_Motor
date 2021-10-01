#include <gtest/gtest.h>

#include <motor/task_system.hpp>

using namespace std;
using namespace motor;

TEST(TaskSystem, Ticker) {
    task_system::timer real_timer;
    task_system::ticker<task_system::ticker_type::CONSTANT> ticker{ real_timer, chrono::milliseconds(5) };

    int counter{ 0 };

    class a : public task_system::tickable {
    public:
        a(task_system::timer const& timer, int& counter)
            : real_timer{ timer }
            , counter{ counter } { }

        task_system::timer const& real_timer;
        int& counter;

        void tick(task_system::delta_time delta) override {
            counter++;
        }
    };
    auto tick_tack_1 = a{ real_timer, counter };

    auto tick_tack_2 = task_system::tickable_object{
        [&real_timer, &counter](task_system::delta_time delta) {
            counter++;
        }
    };

    ticker.tickables().emplace_back(&tick_tack_1);
    ticker.tickables().emplace_back(&tick_tack_2);

    while (real_timer.elapsed() < chrono::milliseconds(23)) {
        ticker.tick();
        this_thread::yield();
    }

    EXPECT_EQ(counter, 8);
}

TEST(TaskSystem, FastTicker) {
    task_system::timer real_timer;
    task_system::ticker<task_system::ticker_type::CONSTANT> ticker{ real_timer, chrono::milliseconds(1) };
    task_system::game_timer game_timer;
    task_system::ticker<task_system::ticker_type::CONSTANT> game_ticker{ game_timer, chrono::milliseconds(20) };
    ticker.tickables().emplace_back(&game_timer);
    game_timer.set_ratio(4.0);
    game_timer.start();

    int counter{ 0 };

    auto tick_tack_1 = task_system::tickable_object{
        [&counter](task_system::delta_time delta) {
            counter++;
        }
    };

    auto tick_tack_2 = task_system::tickable_object{
        [&counter](task_system::delta_time delta) {
            counter++;
        }
    };

    game_ticker.tickables().emplace_back(&tick_tack_1);
    game_ticker.tickables().emplace_back(&tick_tack_2);

    while (real_timer.elapsed() < chrono::milliseconds(23)) {
        ticker.tick();
        game_ticker.tick();
        this_thread::yield();
    }

    EXPECT_EQ(counter, 8);
}

TEST(TaskSystem, SlowTicker) {
    task_system::timer real_timer;
    task_system::ticker<task_system::ticker_type::CONSTANT> ticker{ real_timer, chrono::milliseconds(1) };
    task_system::game_timer game_timer;
    task_system::ticker<task_system::ticker_type::CONSTANT> game_ticker{ game_timer, chrono::microseconds(50) };
    ticker.tickables().emplace_back(&game_timer);
    game_timer.set_ratio(0.01);
    game_timer.start();

    int counter{ 0 };

    auto tick_tack_1 = task_system::tickable_object{
        [&counter](task_system::delta_time delta) {
            counter++;
        }
    };

    auto tick_tack_2 = task_system::tickable_object{
        [&counter](task_system::delta_time delta) {
            counter++;
        }
    };

    game_ticker.tickables().emplace_back(&tick_tack_1);
    game_ticker.tickables().emplace_back(&tick_tack_2);

    while (real_timer.elapsed() < chrono::milliseconds(44)) {
        ticker.tick();
        game_ticker.tick();
        this_thread::yield();
    }

    EXPECT_EQ(counter, 16);
}