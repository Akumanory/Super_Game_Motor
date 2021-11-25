#pragma once

#include <motor/task_system/constants.hpp>
#include <motor/task_system/tickable.hpp>

#include <vector>
#include <functional>

namespace motor {
namespace task_system {

enum class timeline_type {
    VALUE,
    EVENT,
};

template <timeline_type Type, typename ValueType = void>
class timeline : public tickable {
public:
    template <timeline_type, typename ValueType = void>
    struct key_frame;
    template <typename ValueType>
    struct key_frame<timeline_type::VALUE, ValueType> {
        float offset;
        ValueType value;
    };
    template <>
    struct key_frame<timeline_type::EVENT> {
        float offset;
        std::function<void()> event;
    };

public:
    timeline(float time)
        : time_{ time } {
    }
    timeline(timeline const&) = delete;
    timeline(timeline&&) = delete;
    timeline& operator=(timeline const&) = delete;
    timeline& operator=(timeline&&) = delete;
    ~timeline() = default;

public:
    float length() const {
        return length_;
    }
    void set_length(float len) {
        length_ = len;
    }
    void tick(delta_time delta) override {
        if (not launched_) {
            return;
        }
        bool changed{ false };
        size_t key, next_key, prev_key;
        if (reversed_) {
            key = find_prev_(time_);
            time_ -= delta.value;
            if (time_ < 0.f) {
                time_ = std::fmod(time_, length_) + length_;
                prev_key = find_prev_(time_);
                changed = true;
            } else {
                prev_key = find_prev_(time_);
                if (key != prev_key) {
                    changed = true;
                }
            }
            if (not changed) {
                next_key = find_next_(time_);
            }
        } else {
            key = find_next_(time_);
            time_ += delta.value;
            if (time_ > length_) {
                time_ = std::fmod(time_, length_);
                next_key = find_next_(time_);
                changed = true;
            } else {
                next_key = find_next_(time_);
                if (key != next_key) {
                    changed = true;
                }
            }
            if (not changed) {
                prev_key = find_prev_(time_);
            }
        }
        if constexpr (Type == timeline_type::VALUE) {
            if (prev_key == ~size_t{ 0 }) {
                update_(track_[next_key].value);
            } else if (next_key == ~size_t{ 0 }) {
                update_(track_[prev_key].value);
            } else if (next_key == prev_key) {
                update_(track_[prev_key].value);
            } else {
                update_(std::lerp(track_[prev_key].value, track_[next_key].value, time_ - track_[prev_key].offset));
            }
        } else if constexpr (Type == timeline_type::EVENT) {
            if (changed) {
                track_[key].event();
            }
        }
    }
    void play() {
        launched_ = true;
        reversed_ = false;
    }
    void play_from_start() {
        launched_ = true;
        reversed_ = false;
        time_ = 0.f;
    }
    void reverse() {
        launched_ = true;
        reversed_ = true;
    }
    void reverse_from_end() {
        launched_ = true;
        reversed_ = true;
        time_ = length_;
    }
    void stop() {
        launched_ = false;
    }
    void set_time(float time) {
        debug_assert(time >= 0.f and time <= length_);
        time_ = time;
    }
    std::vector<key_frame<Type, ValueType>>& track() {
        return track_;
    }
    std::vector<key_frame<Type, ValueType>> const& track() const {
        return track_;
    }

private:
    size_t find_next_(float time) const {
    }
    size_t find_prev_(float time) const {
    }

private:
    std::vector<key_frame<Type, ValueType>> track_;
    timer const& timer_;
    std::function<void(ValueType)> update_;
    float time_{ 0.f };
    float length_{ 5.f };
    bool looped_{ false };
    bool launched_{ false };
    bool reversed_{ false };
};

} // namespace task_system
} // namespace motor
