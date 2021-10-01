#pragma once

#include <motor/utils.hpp>
#include <motor/task_system/constants.hpp>
#include <motor/task_system/tickable.hpp>

namespace motor {
namespace task_system {

class timer {
public:
    timer() = default;
    timer(timer const&) = default;
    timer(timer&&) = default;
    timer& operator=(timer const&) = default;
    timer& operator=(timer&&) = default;
    virtual ~timer() = default;

public:
    virtual game_clock::duration elapsed() const {
        return game_clock::now() - base_;
    }

protected:
    game_clock::time_point base_{ game_clock::now() };
};

class game_timer : public timer, public tickable {
public:
    game_timer() = default;
    game_timer(game_timer const&) = default;
    game_timer(game_timer&&) = default;
    game_timer& operator=(game_timer const&) = default;
    game_timer& operator=(game_timer&&) = default;
    ~game_timer() override = default;

public:
    virtual void start() {
        active_ = true;
        base_ = game_clock::now();
    }
    virtual void stop() {
        active_ = false;
        total_elapsed_ += last_elapsed_;
        last_elapsed_ = game_clock::duration::zero();
    }
    virtual void reset(bool start) {
        base_ = game_clock::now();
        total_elapsed_ = game_clock::duration::zero();
        last_elapsed_ = game_clock::duration::zero();
        active_ = start;
    }
    void tick(delta_time delta) override {
        if (active_) {
            last_elapsed_ = game_clock::duration{ static_cast<long long>((game_clock::now() - base_).count() * ratio_) };
        }
    }
    game_clock::duration elapsed() const override {
        return total_elapsed_ + last_elapsed_;
    }
    virtual void set_ratio(double ratio) {
        debug_assert(ratio >= MIN_TIMER_RATIO and ratio <= MAX_TIMER_RATIO);
        total_elapsed_ += last_elapsed_;
        last_elapsed_ = game_clock::duration::zero();
        base_ = game_clock::now();
        ratio_ = ratio;
    }

private:
    game_clock::duration last_elapsed_{ game_clock::duration::zero() };
    game_clock::duration total_elapsed_{ game_clock::duration::zero() };
    double ratio_{ INIT_TIMER_RATIO };
    bool active_{ false };
};

} // namespace task_system
} // namespace motor
