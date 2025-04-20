#ifndef OPTION_HPP
#define OPTION_HPP

#include <stdexcept>

template <typename T>
class Option {
    bool hasValue;
    T value;
public:
    Option() : hasValue(false) {}
    Option(const T& val) : hasValue(true), value(val) {}

    static Option<T> Some(const T& val) { return Option<T>(val); }
    static Option<T> None() { return Option<T>(); }

    bool IsSome() const { return hasValue; }
    bool IsNone() const { return !hasValue; }

    T Unwrap() const {
        if (!hasValue) throw std::runtime_error("Unwrap called on None");
        return value;
    }
    T UnwrapOr(const T& defaultVal) const {
        return hasValue ? value : defaultVal;
    }
};

#endif
