#ifndef TYPES_CSTRING_HPP
#define TYPES_CSTRING_HPP

#include <iostream>

// Простая реализация строки без использования <string> или <cstring>
namespace types {

class CString {
    char* data_;
    std::size_t len_;
public:
    // пустая строка
    CString(): data_(nullptr), len_(0) {
        data_ = new char[1];
        data_[0] = '\0';
        len_ = 0;
    }

    // from C-string
    explicit CString(const char* s) {
        if (!s) {
            data_ = new char[1];
            data_[0] = '\0';
            len_ = 0;
            return;
        }
        // compute length manually
        std::size_t l = 0;
        const char* p = s;
        while (*p != '\0') { ++l; ++p; }
        len_ = l;
        data_ = new char[len_ + 1];
        for (std::size_t i = 0; i < len_; ++i) data_[i] = s[i];
        data_[len_] = '\0';
    }

    // copy ctor
    CString(const CString& other) {
        len_ = other.len_;
        data_ = new char[len_ + 1];
        for (std::size_t i = 0; i < len_; ++i) data_[i] = other.data_[i];
        data_[len_] = '\0';
    }

    // move ctor
    CString(CString&& other) noexcept {
        data_ = other.data_;
        len_ = other.len_;
        other.data_ = nullptr;
        other.len_ = 0;
    }

    // copy assign
    CString& operator=(const CString& other) {
        if (this == &other) return *this;
        delete [] data_;
        len_ = other.len_;
        data_ = new char[len_ + 1];
        for (std::size_t i = 0; i < len_; ++i) data_[i] = other.data_[i];
        data_[len_] = '\0';
        return *this;
    }

    // move assign
    CString& operator=(CString&& other) noexcept {
        if (this == &other) return *this;
        delete [] data_;
        data_ = other.data_;
        len_ = other.len_;
        other.data_ = nullptr;
        other.len_ = 0;
        return *this;
    }

    ~CString() {
        if (data_) delete [] data_;
        data_ = nullptr;
        len_ = 0;
    }

    const char* c_str() const { return data_ ? data_ : ""; }
    std::size_t length() const { return len_; }

    // lexicographic compare: returns negative if *this < other, 0 if equal, positive if >
    int compare(const CString& other) const {
        std::size_t i = 0;
        std::size_t n = (len_ < other.len_) ? len_ : other.len_;
        for (; i < n; ++i) {
            unsigned char a = static_cast<unsigned char>(data_[i]);
            unsigned char b = static_cast<unsigned char>(other.data_[i]);
            if (a < b) return -1;
            if (a > b) return 1;
        }
        if (len_ < other.len_) return -1;
        if (len_ > other.len_) return 1;
        return 0;
    }

    bool operator==(const CString& other) const { return compare(other) == 0; }
    bool operator!=(const CString& other) const { return compare(other) != 0; }
    bool operator<(const CString& other) const { return compare(other) < 0; }
    bool operator>(const CString& other) const { return compare(other) > 0; }
    bool operator<=(const CString& other) const { return compare(other) <= 0; }
    bool operator>=(const CString& other) const { return compare(other) >= 0; }

    friend std::ostream& operator<<(std::ostream& os, const CString& s) {
        os << (s.c_str() ? s.c_str() : "");
        return os;
    }
};

} // namespace types

#endif // TYPES_CSTRING_HPP
