#ifndef UTILS_STRINGUTILS_HPP
#define UTILS_STRINGUTILS_HPP

// Мини-реализация strlen/strcpy/strcmp без <cstring>

#include "../core/Common.hpp"

namespace utils {

inline core::index_t my_strlen(const char* s) {
    core::index_t n = 0;
    while (s && s[n] != '\0') ++n;
    return n;
}

inline void my_strcpy(char* dest, const char* src) {
    core::index_t i = 0;
    while (src && src[i] != '\0') {
        dest[i] = src[i];
        ++i;
    }
    dest[i] = '\0';
}

inline int my_strcmp(const char* a, const char* b) {
    core::index_t i = 0;
    while (true) {
        char ca = a ? a[i] : '\0';
        char cb = b ? b[i] : '\0';
        if (ca == '\0' && cb == '\0') return 0;
        if (ca != cb) return (ca < cb) ? -1 : 1;
        ++i;
    }
}

} // namespace utils

#endif // UTILS_STRINGUTILS_HPP
