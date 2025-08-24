#ifndef CORE_ARRAYSEQUENCE_HPP
#define CORE_ARRAYSEQUENCE_HPP

// Реализация Sequence<T> поверх DynamicArray<T>.
// map/where/reduce реализованы как шаблонные методы (принимают callable),
// возвращают ArraySequence<T> (то есть предполагаемая сигнатура map: T->T).
// Это простой и совместимый вариант без std::function.

#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include <utility> // используется только для std::declval в статическом контексте? (не критично)
#include "Common.hpp"
#include "Error.hpp"
#include "Option.hpp"

namespace core {

template<typename T>
class ArraySequence : public Sequence<T> {
    DynamicArray<T> data_;
public:
    ArraySequence() : data_() {}
    ArraySequence(const ArraySequence<T>& other) : data_() {
        for (core::index_t i=0;i<other.size();++i) data_.push_back(other.get(i));
    }

    virtual core::index_t size() const override { return data_.size(); }

    virtual T get(core::index_t idx) const override {
        if (idx >= data_.size()) core::abort_with_error(ErrorCode::INDEX_OUT_OF_RANGE, "ArraySequence::get");
        return data_[idx];
    }

    virtual void set(core::index_t idx, const T& value) override {
        if (idx >= data_.size()) core::abort_with_error(ErrorCode::INDEX_OUT_OF_RANGE, "ArraySequence::set");
        data_[idx] = value;
    }

    virtual void append(const T& value) override { data_.push_back(value); }
    virtual void prepend(const T& value) override { insert(0, value); }
    virtual void insert(core::index_t idx, const T& value) override {
        if (idx > data_.size()) core::abort_with_error(ErrorCode::INDEX_OUT_OF_RANGE, "ArraySequence::insert");
        data_.insert(idx, value);
    }
    virtual void remove_at(core::index_t idx) override {
        data_.remove_at(idx);
    }

    virtual Sequence<T>* concat(const Sequence<T>& other) const override {
        ArraySequence<T>* res = new ArraySequence<T>();
        for (core::index_t i=0;i<size();++i) res->append(get(i));
        for (core::index_t j=0;j<other.size();++j) res->append(other.get(j));
        return res;
    }

    virtual Sequence<T>* subsequence(core::index_t i, core::index_t j) const override {
        if (i > j || j >= size()) core::abort_with_error(ErrorCode::INDEX_OUT_OF_RANGE, "ArraySequence::subsequence");
        ArraySequence<T>* res = new ArraySequence<T>();
        for (core::index_t k = i; k <= j; ++k) res->append(get(k));
        return res;
    }

    virtual Option<core::index_t> find_subsequence(const Sequence<T>& sub) const override {
        core::index_t n = size();
        core::index_t m = sub.size();
        if (m == 0) return Option<core::index_t>::Some(0);
        if (m > n) return Option<core::index_t>::None();
        for (core::index_t i=0;i + m <= n;++i) {
            bool ok = true;
            for (core::index_t j=0;j<m;++j) {
                if (!(get(i+j) == sub.get(j))) { ok = false; break; }
            }
            if (ok) return Option<core::index_t>::Some(i);
        }
        return Option<core::index_t>::None();
    }

    // TEMPLATE map: callable f of signature (const T&, core::index_t) -> T
    template<typename F>
    ArraySequence<T>* map(F f) const {
        ArraySequence<T>* res = new ArraySequence<T>();
        for (core::index_t i=0;i<size();++i) res->append(f(get(i), i));
        return res;
    }

    // TEMPLATE where: callable p(const T&, core::index_t) -> bool
    template<typename P>
    ArraySequence<T>* where(P p) const {
        ArraySequence<T>* res = new ArraySequence<T>();
        for (core::index_t i=0;i<size();++i) {
            if (p(get(i), i)) res->append(get(i));
        }
        return res;
    }

    // TEMPLATE reduce: callable r(const T&, const T&) -> T
    template<typename R>
    Option<T> reduce(R r) const {
        if (size() == 0) return Option<T>::None();
        T acc = get(0);
        for (core::index_t i=1;i<size();++i) acc = r(acc, get(i));
        return Option<T>::Some(acc);
    }
};

} // namespace core

#endif // CORE_ARRAYSEQUENCE_HPP
