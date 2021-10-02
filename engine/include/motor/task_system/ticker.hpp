#pragma once

#include <motor/task_system/constants.hpp>
#include <motor/task_system/timer.hpp>

#include <vector>
#include <memory>

namespace motor {
namespace task_system {

enum class ticker_type {
    CONSTANT,
    VARIABLE,
};

template <ticker_type Type>
class ticker {
public:
    ticker(timer const& timer, game_clock::duration length)
        : timer_{ timer }
        , elapsed_{ timer_.elapsed() }
        , length_{ length } { }
    ticker(ticker const&) = delete;
    ticker(ticker&&) = default;
    ticker& operator=(ticker const&) = delete;
    ticker& operator=(ticker&&) = default;
    ~ticker() = default;

public:
    void tick() {
        auto new_elapsed = timer_.elapsed();
        auto passed_time = new_elapsed - elapsed_;
        while (passed_time >= length_) {
            float delta;
            if constexpr (Type == ticker_type::CONSTANT) {
                delta = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(length_).count()) * 0.001f;
                elapsed_ += length_;
                passed_time -= length_;
            } else {
                delta = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(passed_time).count()) * 0.001f;
                elapsed_ = new_elapsed;
                passed_time = game_clock::duration::zero();
            }
            for (auto i : tickables_) {
                i->tick({ delta });
            }
        }
    }
    std::vector<tickable*>& tickables() {
        return tickables_;
    }

private:
    timer const& timer_;
    game_clock::duration elapsed_;
    game_clock::duration const length_;
    std::vector<tickable*> tickables_;
};

} // namespace task_system
} // namespace motor
