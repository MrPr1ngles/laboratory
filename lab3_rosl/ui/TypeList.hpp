#ifndef UI_TYPELIST_HPP
#define UI_TYPELIST_HPP
#define FOR_EACH_TYPE(X) \
  X(INT,    int)         \
  X(DOUBLE, double)      \
  X(COMPLEX, types::Complex) \
  X(CSTRING, types::CString) \
  X(STUDENT, types::Student) \
  X(TEACHER, types::Teacher)
#endif
