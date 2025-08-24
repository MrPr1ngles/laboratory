#ifndef TYPES_TEACHER_HPP
#define TYPES_TEACHER_HPP

#include "Person.hpp"

namespace types {

struct Teacher : public Person {
    int teacherId;
    types::CString department;

    Teacher(): Person(), teacherId(0), department("") {}
    Teacher(int id, const types::CString& f, const types::CString& m, const types::CString& l, const types::CString& b, const types::CString& dept)
      : Person(f,m,l,b), teacherId(id), department(dept) {}

    bool operator==(const Teacher& o) const {
        return teacherId == o.teacherId;
    }

    bool operator<(const Teacher& o) const {
        int cmp = lastName.compare(o.lastName);
        if (cmp < 0) return true;
        if (cmp > 0) return false;
        return teacherId < o.teacherId;
    }
};

} // namespace types

#endif // TYPES_TEACHER_HPP
