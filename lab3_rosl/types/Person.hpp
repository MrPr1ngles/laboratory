#ifndef TYPES_PERSON_HPP
#define TYPES_PERSON_HPP

// Базовая структура Person; Student и Teacher наследуют.

#include "CString.hpp"

namespace types {

struct Person {
    types::CString firstName;
    types::CString middleName;
    types::CString lastName;
    types::CString birthDate; // как строка "YYYY-MM-DD"

    Person() = default;
    Person(const types::CString& f, const types::CString& m, const types::CString& l, const types::CString& b)
      : firstName(f), middleName(m), lastName(l), birthDate(b) {}
};

} // namespace types

#endif // TYPES_PERSON_HPP
