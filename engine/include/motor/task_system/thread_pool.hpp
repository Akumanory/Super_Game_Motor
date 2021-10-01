#pragma once

#include <motor/task_system/constants.hpp>

#include <utility>
#include <thread>
#include <vector>
#include <deque>
#include <mutex>
#include <future>

namespace motor {
namespace task_system {

class thread_pool {
public:
    thread_pool();
    explicit thread_pool(
      int32_t threads_count,
      uint64_t affinity_mask = ~0ull,
      std::vector<std::string_view> thread_names = {});

    thread_pool(thread_pool const&) = delete;
    thread_pool(thread_pool&&) = default;
    thread_pool& operator=(thread_pool const&) = delete;
    thread_pool& operator=(thread_pool&&) = default;

    ~thread_pool();

public:
    void submit(std::function<void()> fn);
    void submit(std::function<void(std::stop_token)>);
    void join();
    void stop();

private:
    void init_();
    void thread_loop_(std::stop_token stop_token, int32_t thread_id);
    void set_affinity_();
    void set_name_(std::string_view);

private:
    std::vector<std::jthread> threads_;
    int32_t threads_count_{ MAX_THREADS_COUNT };
    uint64_t affinity_mask_{ ~0ull };
    std::deque<std::function<void(std::stop_token)>> fn_queue_;
    std::mutex mtx_;
    std::condition_variable cond_var_;
    std::atomic_bool no_more_tasks_{ false };
    std::atomic_bool stop_requested_{ false };
};

} // namespace task_system
} // namespace motor
