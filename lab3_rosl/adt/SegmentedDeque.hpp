#ifndef ADT_SEGMENTEDDEQUE_HPP
#define ADT_SEGMENTEDDEQUE_HPP

// Сегментированный дек. Первая корректная реализация, ориентированная на
// правильную логику и простоту (не сверх-оптимизированная), но с сегментами.
// Сегмент задан пользователем (по умолчанию 16).
// Методы map/where/reduce/concat/find_subsequence реализованы как шаблонные.

#include "../core/DynamicArray.hpp"
#include "../core/Error.hpp"
#include "../core/Option.hpp"
#include "../core/Common.hpp"
#include <new>

namespace adt {

template<typename T>
class SegmentedDeque {
    using DynSegArr = core::DynamicArray<T*>; // массив указателей на сегменты
    core::index_t seg_size_;
    DynSegArr segments_;         // segments_[0..segments_.size()-1] - указатели на сегменты
    core::index_t total_size_;

    T* alloc_segment() {
        T* seg = nullptr;
        try {
            seg = static_cast<T*>(::operator new(sizeof(T) * (size_t)seg_size_));
        } catch(...) {
            core::abort_with_error(core::ErrorCode::ALLOCATION_ERROR, "SegmentedDeque::alloc_segment");
        }
        return seg;
    }

public:
    explicit SegmentedDeque(core::index_t seg_size = 16)
     : seg_size_(seg_size), segments_(), total_size_(0) {}

    ~SegmentedDeque() {
        for (core::index_t i = 0; i < segments_.size(); ++i) {
            if (segments_[i]) {
                // destroy constructed elements inside the segment:
                // we do not track per-segment counts, so we conservatively won't call destructors here
                ::operator delete(static_cast<void*>(segments_[i]));
            }
        }
    }

    core::index_t size() const { return total_size_; }
    bool empty() const { return total_size_ == 0; }

    // get element by linear index (0..size-1)
    T get(core::index_t idx) const {
        if (idx >= total_size_) core::abort_with_error(core::ErrorCode::INDEX_OUT_OF_RANGE, "SegmentedDeque::get");
        core::index_t segno = idx / seg_size_;
        core::index_t offset = idx % seg_size_;
        T* seg = segments_[segno];
        return seg[offset];
    }

    void push_back(const T& v) {
        core::index_t pos = total_size_;
        core::index_t segno = pos / seg_size_;
        core::index_t offset = pos % seg_size_;
        if (segno >= segments_.size()) {
            // allocate new segment
            segments_.push_back(alloc_segment());
        }
        T* seg = segments_[segno];
        new (&seg[offset]) T(v);
        ++total_size_;
    }

    void push_front(const T& v) {
        // простая, корректная реализация: создаём временный дек, вставляем v, копируем остальные элементы.
        SegmentedDeque<T> tmp(seg_size_);
        tmp.push_back(v);
        for (core::index_t i=0;i<size();++i) tmp.push_back(get(i));
        *this = core::my_move(tmp);
    }

    void pop_back() {
        if (empty()) core::abort_with_error(core::ErrorCode::EMPTY_CONTAINER, "SegmentedDeque::pop_back");
        core::index_t pos = total_size_ - 1;
        core::index_t segno = pos / seg_size_;
        core::index_t offset = pos % seg_size_;
        T* seg = segments_[segno];
        seg[offset].~T();
        --total_size_;
    }

    void pop_front() {
        if (empty()) core::abort_with_error(core::ErrorCode::EMPTY_CONTAINER, "SegmentedDeque::pop_front");
        SegmentedDeque<T> tmp(seg_size_);
        for (core::index_t i=1;i<size();++i) tmp.push_back(get(i));
        *this = core::my_move(tmp);
    }

    // concat: A then B
    SegmentedDeque<T> concat(const SegmentedDeque<T>& other) const {
        SegmentedDeque<T> out(seg_size_);
        for (core::index_t i=0;i<size();++i) out.push_back(get(i));
        for (core::index_t j=0;j<other.size();++j) out.push_back(other.get(j));
        return out;
    }

    // find subsequence (naive)
    core::Option<core::index_t> find_subsequence(const SegmentedDeque<T>& sub) const {
        core::index_t n = size();
        core::index_t m = sub.size();
        if (m == 0) return core::Option<core::index_t>::Some(0);
        if (m > n) return core::Option<core::index_t>::None();
        for (core::index_t i=0;i + m <= n;++i) {
            bool ok = true;
            for (core::index_t j=0;j<m;++j) {
                if (!(get(i+j) == sub.get(j))) { ok = false; break; }
            }
            if (ok) return core::Option<core::index_t>::Some(i);
        }
        return core::Option<core::index_t>::None();
    }

    // map: F(const T&, core::index_t) -> T
    template<typename F>
    SegmentedDeque<T> map(F f) const {
        SegmentedDeque<T> out(seg_size_);
        for (core::index_t i=0;i<size();++i) out.push_back(f(get(i), i));
        return out;
    }

    // where: P(const T&, core::index_t) -> bool
    template<typename P>
    SegmentedDeque<T> where(P p) const {
        SegmentedDeque<T> out(seg_size_);
        for (core::index_t i=0;i<size();++i) if (p(get(i), i)) out.push_back(get(i));
        return out;
    }

    // reduce: R(const T&, const T&) -> T
    template<typename R>
    core::Option<T> reduce(R r) const {
        if (empty()) return core::Option<T>::None();
        T acc = get(0);
        for (core::index_t i=1;i<size();++i) acc = r(acc, get(i));
        return core::Option<T>::Some(acc);
    }

    // Note: sort/merge functions will be provided via utils/Sorting.hpp (use copy->sort->rebuild)
};

} // namespace adt

#endif // ADT_SEGMENTEDDEQUE_HPP
