#ifndef UI_ADTDEFS_HPP
#define UI_ADTDEFS_HPP

// Общие определения для UI: перечисления ADTKind, DataKind и структура ADTEntry.
// Эти определения подключаются в файлах ui/*.hpp/.cpp, которые работают с хранилищем ADT.

#include <cstddef> // только для std::size_t если понадобится

namespace ui {

// Тип ADT: стек или сегментированный дек
enum class ADTKind { STACK = 1, DEQUE = 2 };

// Доступные типы данных (соответствуют индексации в реестре типов)
enum class DataKind {
    INT = 1,
    DOUBLE = 2,
    COMPLEX = 3,
    CSTRING = 4,
    STUDENT = 5,
    TEACHER = 6
};

// Запись об одном созданном ADT: какой ADT, какой тип данных и указатель на объект.
// obj хранит либо Stack<T>* либо SegmentedDeque<T>* (как void*).
struct ADTEntry {
    ADTKind adt;
    DataKind dt;
    void* obj;
    ADTEntry(): adt(ADTKind::STACK), dt(DataKind::INT), obj(nullptr) {}
};

} // namespace ui

#endif // UI_ADTDEFS_HPP
