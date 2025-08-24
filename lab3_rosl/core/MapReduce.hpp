#ifndef CORE_MAPREDUCE_HPP
#define CORE_MAPREDUCE_HPP

// Здесь можно разместить общие шаблонные адаптеры map/where/reduce,
// если понадобится. Для текущей реализации конкретные Sequence-классы
// содержат свои шаблонные методы map/where/reduce, поэтому файл служит
// как заглушка/место для общих утилит.

#include "Common.hpp"

namespace core {

// пример утилиты: apply f to each element of Sequence via get/size interface
template<typename Seq, typename F>
void for_each(const Seq& s, F f) {
    for (core::index_t i=0;i<s.size();++i) {
        f(s.get(i), i);
    }
}

} // namespace core

#endif // CORE_MAPREDUCE_HPP
