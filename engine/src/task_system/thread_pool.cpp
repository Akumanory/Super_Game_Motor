#include <motor/task_system/thread_pool.hpp>
#include <motor/task_system/constants.hpp>
#include <motor/utils.hpp>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef min
#undef max

using namespace motor;
using namespace task_system;

thread_pool::thread_pool(int32_t threads_count, uint64_t affinity_mask, std::vector<std::string_view> thread_names)
    : threads_{ threads_count }
    , threads_count_{ threads_count }
    , affinity_mask_{ affinity_mask } {
    debug_assert(threads_count > 0);
}

thread_pool::~thread_pool() {
    join();
}

void thread_pool::submit(std::function<void()> fn) {
    submit(
      [fn = std::move(fn)](std::stop_token) {
          fn();
      });
}

void thread_pool::submit(std::function<void(std::stop_token)> fn) {
    if (no_more_tasks_ or stop_requested_) {
        throw std::logic_error{ "can't submit task, thread pool is closed" };
    }
    auto lg = std::lock_guard{ mtx_ };
    fn_queue_.emplace_back(std::move(fn));
    cond_var_.notify_one();
}

void thread_pool::join() {
    no_more_tasks_ = true;
    cond_var_.notify_all();
    for (auto& thr : threads_) {
        if (thr.joinable()) {
            thr.join();
        }
    }
}
void thread_pool::stop() {
    no_more_tasks_ = true;
    stop_requested_ = true;
    for (auto& thr : threads_) {
        thr.request_stop();
    }
    cond_var_.notify_all();
}

void thread_pool::init_() {
    for (auto i = 0; i < threads_count_; ++i) {
        threads_[i] = std::jthread{
            [this, i](std::stop_token stop_token) {
                set_affinity_();
                thread_loop_(stop_token, i);
            }
        };
    }
}

void thread_pool::thread_loop_(std::stop_token stop_token, int32_t thread_id) {
    auto lock = std::unique_lock{ mtx_, std::defer_lock };
    while (not stop_token.stop_requested()) {

        lock.lock();
        cond_var_.wait(lock, [this, &stop_token] {
            return not fn_queue_.empty() or stop_token.stop_requested() or no_more_tasks_;
        });
        if (stop_token.stop_requested() or no_more_tasks_ and fn_queue_.empty()) {
            break;
        }
        auto fn = std::move(fn_queue_.front());
        fn_queue_.pop_front();
        lock.unlock();

        fn(stop_token);
        std::this_thread::yield();
    }
}

void thread_pool::set_affinity_() {
    auto handle = GetCurrentThread();
    SetThreadAffinityMask(handle, affinity_mask_);
}
