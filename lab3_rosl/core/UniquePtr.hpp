#ifndef CORE_UNIQUEPTR_HPP
#define CORE_UNIQUEPTR_HPP

// Минимальная простая реализация unique_ptr,
// чтобы не использовать <memory>.
// Только необходимая функциональность: ctor, dtor, move, reset, release, get.

namespace core {

template<typename T>
class UniquePtr {
    T* ptr_;
public:
    constexpr UniquePtr() noexcept : ptr_(nullptr) {}
    explicit UniquePtr(T* p) noexcept : ptr_(p) {}
    ~UniquePtr() { if (ptr_) delete ptr_; }

    // move
    UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) { other.ptr_ = nullptr; }
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            if (ptr_) delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    // copy deleted
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    T* get() const noexcept { return ptr_; }
    T* release() noexcept { T* tmp = ptr_; ptr_ = nullptr; return tmp; }
    void reset(T* p = nullptr) noexcept {
        if (ptr_) delete ptr_;
        ptr_ = p;
    }

    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }

    explicit operator bool() const noexcept { return ptr_ != nullptr; }
};

} // namespace core

#endif // CORE_UNIQUEPTR_HPP
