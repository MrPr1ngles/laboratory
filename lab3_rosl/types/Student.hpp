#ifndef TYPES_STUDENT_HPP
#define TYPES_STUDENT_HPP

#include "Person.hpp"
#include "../utils/StringUtils.hpp"

namespace types {

struct Student : public Person {
    int studentId;
    double gpa;

    Student(): Person(), studentId(0), gpa(0.0) {}
    Student(int id, const types::CString& f, const types::CString& m, const types::CString& l, const types::CString& b, double g)
      : Person(f,m,l,b), studentId(id), gpa(g) {}
    
    bool operator==(const Student& o) const {
        return studentId == o.studentId;
    }

    bool operator<(const Student& o) const {
        // сортировка по фамилии (CString::compare), затем по id
        int cmp = lastName.compare(o.lastName);
        if (cmp < 0) return true;
        if (cmp > 0) return false;
        return studentId < o.studentId;
    }
};

} // namespace types

#endif // TYPES_STUDENT_HPP
