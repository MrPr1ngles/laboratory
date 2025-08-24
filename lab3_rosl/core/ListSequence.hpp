#ifndef CORE_LISTSEQUENCE_HPP
#define CORE_LISTSEQUENCE_HPP

// Реализация Sequence<T> через LinkedList<T>.
// Тоже содержит шаблонные map/where/reduce.

#include "Sequence.hpp"
#include "LinkedList.hpp"
#include "Option.hpp"
#include "Common.hpp"

namespace core {

template<typename T>
class ListSequence : public Sequence<T> {
    LinkedList<T> list_;
public:
    ListSequence() = default;

    virtual core::index_t size() const override { return list_.size(); }
    virtual T get(core::index_t idx) const override { return list_.get(idx); }
    virtual void set(core::index_t idx, const T& value) override {
        // неэффективно: извлечь, заменить, вставить — проще: пройти до узла -> value = value
        // Но LinkedList::Node не открыт; для простоты — удалить+insert
        // Однако это сломает порядок. Поэтому здесь сделаем простую корректную реализацию:
        core::index_t n = size();
        if (idx >= n) core::abort_with_error(ErrorCode::INDEX_OUT_OF_RANGE, "ListSequence::set");
        LinkedList<T> tmp;
        for (core::index_t i=0;i<n;++i) {
            T v = list_.get(i);
            if (i == idx) tmp.append(value); else tmp.append(v);
        }
        list_ = core::my_move(tmp);
    }
    virtual void append(const T& value) override { list_.append(value); }
    virtual void prepend(const T& value) override { list_.prepend(value); }
    virtual void insert(core::index_t idx, const T& value) override { list_.insert(idx, value); }
    virtual void remove_at(core::index_t idx) override { list_.remove_at(idx); }

    virtual Sequence<T>* concat(const Sequence<T>& other) const override {
        ListSequence<T>* res = new ListSequence<T>();
        for (core::index_t i=0;i<size();++i) res->append(get(i));
        for (core::index_t j=0;j<other.size();++j) res->append(other.get(j));
        return res;
    }

    virtual Sequence<T>* subsequence(core::index_t i, core::index_t j) const override {
        if (i > j || j >= size()) core::abort_with_error(ErrorCode::INDEX_OUT_OF_RANGE, "ListSequence::subsequence");
        ListSequence<T>* res = new ListSequence<T>();
        for (core::index_t k = i; k <= j; ++k) res->append(get(k));
        return res;
    }

    virtual Option<core::index_t> find_subsequence(const Sequence<T>& sub) const override {
        core::index_t n = size(), m = sub.size();
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

    template<typename F>
    ListSequence<T>* map(F f) const {
        ListSequence<T>* res = new ListSequence<T>();
        for (core::index_t i=0;i<size();++i) res->append(f(get(i), i));
        return res;
    }

    template<typename P>
    ListSequence<T>* where(P p) const {
        ListSequence<T>* res = new ListSequence<T>();
        for (core::index_t i=0;i<size();++i) if (p(get(i), i)) res->append(get(i));
        return res;
    }

    template<typename R>
    Option<T> reduce(R r) const {
        if (size() == 0) return Option<T>::None();
        T acc = get(0);
        for (core::index_t i=1;i<size();++i) acc = r(acc, get(i));
        return Option<T>::Some(acc);
    }
};

} // namespace core

#endif // CORE_LISTSEQUENCE_HPP
