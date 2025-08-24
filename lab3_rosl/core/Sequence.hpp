#ifndef CORE_SEQUENCE_HPP
#define CORE_SEQUENCE_HPP

// Абстрактный интерфейс Sequence<T>.
// ВНИМАНИЕ: методы map/where/reduce НЕ являются виртуальными и реализованы
// в конкретных реализациях как шаблонные методы.

#include "Common.hpp"
#include "Option.hpp"
#include "Error.hpp"

namespace core {

template<typename T>
class Sequence {
public:
    virtual ~Sequence() = default;

    virtual core::index_t size() const = 0;
    virtual T get(core::index_t idx) const = 0;
    virtual void set(core::index_t idx, const T& value) = 0;
    virtual void append(const T& value) = 0;
    virtual void prepend(const T& value) = 0;
    virtual void insert(core::index_t idx, const T& value) = 0;
    virtual void remove_at(core::index_t idx) = 0;

    virtual Sequence<T>* concat(const Sequence<T>& other) const = 0;
    virtual Sequence<T>* subsequence(core::index_t i, core::index_t j) const = 0; // inclusive
    virtual Option<core::index_t> find_subsequence(const Sequence<T>& sub) const = 0;
};

} // namespace core

#endif // CORE_SEQUENCE_HPP
