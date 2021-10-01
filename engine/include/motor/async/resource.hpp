#pragma once

#include <type_traits>
#include <concepts>
#include <shared_mutex>

namespace motor {
namespace async {

template <typename ResourceType>
class shared_resource {
public:
    shared_resource() = default;
    template <typename... Args>
    shared_resource(Args&&... args) {
        resource_ = new ((void*)::std::addressof(storage_)) ResourceType{ std::forward<Args>(args)... };
    }
    shared_resource(shared_resource const& other) {
        std::shared_lock other_lock(other.mtx_);
        resource_ = new ((void*)::std::addressof(storage_)) ResourceType{ *other.resource_ };
    }
    shared_resource(shared_resource&& other) {
        std::unique_lock other_lock(other.mtx_);
        resource_ = new ((void*)::std::addressof(storage_)) ResourceType{ std::move(*other.resource_) };
        other.destroy_();
    }
    shared_resource& operator=(shared_resource const& other) {
        std::unique_lock lock(mtx_);
        {
            std::shared_lock other_lock(other.mtx_);
            resource_ = new ((void*)::std::addressof(storage_)) ResourceType{ *other.resource_ };
        }
        return *this;
    }
    shared_resource& operator=(shared_resource&& other) {
        std::unique_lock lock(mtx_);
        {
            std::unique_lock other_lock(other.mtx_);
            resource_ = new ((void*)::std::addressof(storage_)) ResourceType{ *other.resource_ };
            other.destroy_();
        }
        return *this;
    }
    ~shared_resource() {
        destroy();
    }

public:
    template <typename... Args>
    void init(Args&&... args) {
        std::unique_lock lock(mtx_);
        destroy_();
        resource_ = new ((void*)::std::addressof(storage_)) ResourceType{ std::forward<Args>(args)... };
    }

    void destroy() {
        std::unique_lock lock(mtx_);
        destroy_();
    }

    template <std::invocable<ResourceType const&> F>
    void read(F&& f) const {
        std::shared_lock lock(mtx_);
        f(*resource_);
    }

    template <std::invocable<ResourceType&> F>
    void write(F&& f) {
        std::unique_lock lock(mtx_);
        f(*resource_);
    }

    explicit operator bool() const noexcept {
        return resource_ != nullptr;
    }

private:
    void destroy_() {
        if (resource_) {
            resource_->~ResourceType();
            resource_ = nullptr;
        }
    }

private:
    mutable std::shared_mutex mtx_;
    ResourceType* resource_{ nullptr };
    std::aligned_storage_t<sizeof(ResourceType), alignof(ResourceType)> storage_{};
};

} // namespace async
} // namespace motor
