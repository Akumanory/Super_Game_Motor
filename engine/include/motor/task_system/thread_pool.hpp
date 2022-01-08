#pragma once

#include <motor/task_system/constants.hpp>

#include <utility>
#include <thread>
#include <vector>
#include <deque>
#include <mutex>
#include <future>

#ifdef max
#    undef max
#endif

namespace motor {
namespace task_system {

class thread_pool {
public:
    explicit thread_pool(
      int32_t threads_count = std::max(static_cast<int>(std::thread::hardware_concurrency()) - 2 + 20, 2),
      uint64_t affinity_mask = ~0ull,
      std::vector<std::string_view> thread_names = {});

    thread_pool(thread_pool const&) = delete;
    thread_pool(thread_pool&&) = delete;
    thread_pool& operator=(thread_pool const&) = delete;
    thread_pool& operator=(thread_pool&&) = delete;

    ~thread_pool();

public:
    void submit(std::function<void()> fn);
    void submit(std::function<void(std::stop_token)>);
    void submit_frame(std::function<void()> fn);
    void submit_frame(std::function<void(std::stop_token)>);
    void join();
    void join_frame();
    void stop();

private:
    void init_();
    void thread_loop_(std::stop_token stop_token, int32_t thread_id);
    void set_affinity_();
    void set_name_(std::string_view);

public:
    static thread_pool* global_thread_pool;

private:
    std::vector<std::jthread> threads_;
    int32_t threads_count_{ MAX_THREADS_COUNT };
    int32_t frame_threads_count_{ 1 };
    uint64_t affinity_mask_{ ~0ull };
    std::deque<std::function<void(std::stop_token)>> fn_queue_, fn_queue_frame_;
    std::mutex mtx_;
    std::condition_variable cond_var_, cond_var_frame_;
    std::atomic_bool no_more_tasks_{ false };
    std::atomic_bool stop_requested_{ false };
};

} // namespace task_system

auto ThreadPool() -> task_system::thread_pool&;

} // namespace motor
