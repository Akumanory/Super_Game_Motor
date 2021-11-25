#pragma once

namespace motor {
namespace task_system {

#include <concepts>
#include <functional>

struct delta_time {
    float value;
};

class tickable {
public:
    virtual ~tickable() = 0;
    virtual void tick(delta_time) = 0;
};

inline tickable::~tickable() { }

class tickable_object : public tickable {
public:
    template <std::invocable<delta_time> F>
    explicit tickable_object(F&& f)
        : f_{ std::forward<F>(f) } { }

    tickable_object(tickable_object const&) = default;
    tickable_object(tickable_object&&) = default;
    tickable_object& operator=(tickable_object const&) = default;
    tickable_object& operator=(tickable_object&&) = default;
    ~tickable_object() override = default;

    void tick(delta_time delta) override {
        f_(delta);
    }

private:
    std::function<void(delta_time)> f_;
};

} // namespace task_system
} // namespace motor