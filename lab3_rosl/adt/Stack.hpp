#ifndef ADT_STACK_HPP
#define ADT_STACK_HPP

// Стек реализован поверх ArraySequence<T> (композиция).
// Для простоты и надёжности здесь используется сырой указатель на
// core::ArraySequence<T> с явным управлением жизненным циклом.
// Реализованы корректные конструктор, деструктор, копирование и перемещение.
// Методы map/where/reduce реализованы как шаблонные методы Stack.

#include "../core/ArraySequence.hpp"
#include "../core/Error.hpp"
#include "../core/Option.hpp"
#include "../core/Common.hpp"

namespace adt {

template<typename T>
class Stack {
    core::ArraySequence<T>* seq_;
public:
    // default ctor
    Stack(): seq_(new core::ArraySequence<T>()) {}

    // copy ctor
    Stack(const Stack<T>& other) : seq_(new core::ArraySequence<T>(*other.seq_)) {}

    // move ctor
    Stack(Stack<T>&& other) noexcept : seq_(other.seq_) {
        other.seq_ = nullptr;
    }

    // destructor
    ~Stack() {
        if (seq_) delete seq_;
    }

    // copy assignment
    Stack& operator=(const Stack<T>& other) {
        if (this != &other) {
            if (seq_) delete seq_;
            seq_ = new core::ArraySequence<T>(*other.seq_);
        }
        return *this;
    }

    // move assignment
    Stack& operator=(Stack<T>&& other) noexcept {
        if (this != &other) {
            if (seq_) delete seq_;
            seq_ = other.seq_;
            other.seq_ = nullptr;
        }
        return *this;
    }

    core::index_t size() const {
        if (!seq_) return 0;
        return seq_->size();
    }

    bool empty() const {
        return size() == 0;
    }

    void push(const T& v) {
        if (!seq_) seq_ = new core::ArraySequence<T>();
        seq_->append(v);
    }

    void push(T&& v) {
        if (!seq_) seq_ = new core::ArraySequence<T>();
        seq_->append(core::my_move(v));
    }

    void pop() {
        if (empty()) core::abort_with_error(core::ErrorCode::EMPTY_CONTAINER, "Stack::pop");
        seq_->remove_at(seq_->size() - 1);
    }

    T top() const {
        if (empty()) core::abort_with_error(core::ErrorCode::EMPTY_CONTAINER, "Stack::top");
        return seq_->get(seq_->size() - 1);
    }

    // direct access by index (bottom=0 .. top=size-1)
    T at(core::index_t idx) const {
        if (!seq_) core::abort_with_error(core::ErrorCode::EMPTY_CONTAINER, "Stack::at");
        return seq_->get(idx);
    }

    // concat: A + B => elements of A then B (bottom->top preserved)
    Stack<T> concat(const Stack<T>& other) const {
        core::ArraySequence<T>* newArr = new core::ArraySequence<T>();
        for (core::index_t i = 0; i < size(); ++i) newArr->append(seq_->get(i));
        for (core::index_t j = 0; j < other.size(); ++j) newArr->append(other.seq_->get(j));
        Stack<T> res;
        // replace internal seq_ of res with newArr
        if (res.seq_) delete res.seq_;
        res.seq_ = newArr;
        return res;
    }

    // subsequence returns Stack with elements i..j inclusive (bottom->top preserved)
    Stack<T> subsequence(core::index_t i, core::index_t j) const {
        if (i > j || j >= size()) core::abort_with_error(core::ErrorCode::INDEX_OUT_OF_RANGE, "Stack::subsequence");
        core::ArraySequence<T>* newArr = new core::ArraySequence<T>();
        for (core::index_t k = i; k <= j; ++k) newArr->append(seq_->get(k));
        Stack<T> res;
        if (res.seq_) delete res.seq_;
        res.seq_ = newArr;
        return res;
    }

    core::Option<core::index_t> find_subsequence(const Stack<T>& sub) const {
        return seq_->find_subsequence(*sub.seq_);
    }

    // map/where/reduce (order: bottom -> top). Callable signatures:
    // map: F(const T&, core::index_t) -> T
    template<typename F>
    Stack<T> map(F f) const {
        core::ArraySequence<T>* mapped = seq_->template map<F>(f);
        Stack<T> res;
        if (res.seq_) delete res.seq_;
        res.seq_ = mapped;
        return res;
    }

    // where: P(const T&, core::index_t) -> bool
    template<typename P>
    Stack<T> where(P p) const {
        core::ArraySequence<T>* filtered = seq_->template where<P>(p);
        Stack<T> res;
        if (res.seq_) delete res.seq_;
        res.seq_ = filtered;
        return res;
    }

    // reduce: R(const T&, const T&) -> T
    template<typename R>
    core::Option<T> reduce(R r) const {
        return seq_->template reduce<R>(r);
    }
};

} // namespace adt

#endif // ADT_STACK_HPP
