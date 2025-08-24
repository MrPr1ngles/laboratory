#ifndef CORE_OPTION_HPP
#define CORE_OPTION_HPP

#include <utility>

namespace core {

template<typename T>
class Option {
    bool present_;
    T value_;
public:
    Option(): present_(false) {}
    Option(const T& v): present_(true), value_(v) {}
    Option(T&& v): present_(true), value_(std::move(v)) {}

    static Option<T> None() { return Option<T>(); }
    static Option<T> Some(const T& v) { return Option<T>(v); }
    static Option<T> Some(T&& v) { return Option<T>(std::move(v)); }

    bool has_value() const { return present_; }
    T& value() { return value_; }
    const T& value() const { return value_; }
};

} // namespace core

#endif // CORE_OPTION_HPP
