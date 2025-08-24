#ifndef UI_TYPE_IO_HPP
#define UI_TYPE_IO_HPP

// Шаблонные функции ввода/печати элементов типов, используемых в UI.
// Не использует std::function; использует только i/o и std::string.
// Специализации: int, double, types::Complex, types::CString, types::Student, types::Teacher.

#include <iostream>
#include <limits>
#include <string>

#include "../types/Complex.hpp"
#include "../types/CString.hpp"
#include "../types/Student.hpp"
#include "../types/Teacher.hpp"
#include "../core/Common.hpp"

namespace ui {

// Общие безопасные чтения
inline bool read_int_safe(int &out) {
    if (!(std::cin >> out)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    return true;
}

inline bool read_index(core::index_t &out) {
    unsigned long x;
    if (!(std::cin >> x)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    out = static_cast<core::index_t>(x);
    return true;
}

// read_one / print_one: generic prototypes
template<typename T> T read_one();
template<typename T> void print_one(const T&);

// --- int
template<> inline int read_one<int>() {
    int v;
    while (!read_int_safe(v)) std::cout << "Invalid integer, try again: ";
    return v;
}
template<> inline void print_one<int>(const int& v) { std::cout << v; }

// --- double
template<> inline double read_one<double>() {
    double v;
    while (!(std::cin >> v)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid number, try again: ";
    }
    return v;
}
template<> inline void print_one<double>(const double& v) { std::cout << v; }

// --- Complex (types::Complex)
template<> inline types::Complex read_one<types::Complex>() {
    double r, im;
    while (!(std::cin >> r >> im)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid complex, enter real and imag: ";
    }
    return types::Complex(r, im);
}
template<> inline void print_one<types::Complex>(const types::Complex& c) {
    std::cout << c;
}

// --- CString
template<> inline types::CString read_one<types::CString>() {
    std::string s;
    std::cin >> s;
    return types::CString(s.c_str());
}
template<> inline void print_one<types::CString>(const types::CString& s) {
    std::cout << s.c_str();
}

// --- Student
template<> inline types::Student read_one<types::Student>() {
    int id;
    std::cout << "Student id: ";
    while (!read_int_safe(id)) std::cout << "Invalid id: ";
    std::string first, middle, last, birth;
    double gpa;
    std::cout << "First name: "; std::cin >> first;
    std::cout << "Middle name (or -): "; std::cin >> middle;
    std::cout << "Last name: "; std::cin >> last;
    std::cout << "Birth (YYYY-MM-DD): "; std::cin >> birth;
    std::cout << "GPA: ";
    while (!(std::cin >> gpa)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::cout << "Invalid gpa: "; }
    return types::Student(id, types::CString(first.c_str()), types::CString(middle.c_str()), types::CString(last.c_str()), types::CString(birth.c_str()), gpa);
}
template<> inline void print_one<types::Student>(const types::Student& s) {
    std::cout << "[" << s.studentId << "] " << s.firstName.c_str() << " " << s.middleName.c_str() << " " << s.lastName.c_str() << " GPA:" << s.gpa;
}

// --- Teacher
template<> inline types::Teacher read_one<types::Teacher>() {
    int id;
    std::cout << "Teacher id: ";
    while (!read_int_safe(id)) std::cout << "Invalid id: ";
    std::string first, middle, last, birth, dept;
    std::cout << "First name: "; std::cin >> first;
    std::cout << "Middle name (or -): "; std::cin >> middle;
    std::cout << "Last name: "; std::cin >> last;
    std::cout << "Birth (YYYY-MM-DD): "; std::cin >> birth;
    std::cout << "Department: "; std::cin >> dept;
    return types::Teacher(id, types::CString(first.c_str()), types::CString(middle.c_str()), types::CString(last.c_str()), types::CString(birth.c_str()), types::CString(dept.c_str()));
}
template<> inline void print_one<types::Teacher>(const types::Teacher& t) {
    std::cout << "[" << t.teacherId << "] " << t.firstName.c_str() << " " << t.lastName.c_str() << " Dept:" << t.department.c_str();
}

} // namespace ui

#endif // UI_TYPE_IO_HPP
