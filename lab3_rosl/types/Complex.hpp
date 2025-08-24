#ifndef TYPES_COMPLEX_HPP
#define TYPES_COMPLEX_HPP

// Простая реализация комплексного числа без <cmath>, <cstdio>, <string>.
// Реализованы операции + - * /, ==, !=, <, > и потоковый вывод operator<<.

#include <iostream>

namespace types {

class Complex {
    double re_;
    double im_;
public:
    Complex(): re_(0.0), im_(0.0) {}
    Complex(double r, double i = 0.0): re_(r), im_(i) {}

    double re() const { return re_; }
    double im() const { return im_; }

    Complex operator+(const Complex& o) const { return Complex(re_ + o.re_, im_ + o.im_); }
    Complex operator-(const Complex& o) const { return Complex(re_ - o.re_, im_ - o.im_); }
    Complex operator*(const Complex& o) const {
        return Complex(re_*o.re_ - im_*o.im_, re_*o.im_ + im_*o.re_);
    }
    Complex operator/(const Complex& o) const {
        double denom = o.re_*o.re_ + o.im_*o.im_;
        // простая защита от деления на ноль
        if (denom == 0.0) denom = 1e-18;
        return Complex((re_*o.re_ + im_*o.im_)/denom, (im_*o.re_ - re_*o.im_)/denom);
    }

    bool operator==(const Complex& o) const {
        return re_ == o.re_ && im_ == o.im_;
    }
    bool operator!=(const Complex& o) const { return !(*this == o); }

    // Сравнение по квадрату модуля (без использования <cmath>)
    bool operator<(const Complex& o) const {
        double m1 = re_*re_ + im_*im_;
        double m2 = o.re_*o.re_ + o.im_*o.im_;
        return m1 < m2;
    }
    bool operator>(const Complex& o) const { return o < *this; }

    // Печать в поток: "a+bi" или "a-bi"
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << c.re_;
        if (c.im_ >= 0) os << "+" << c.im_ << "i";
        else os << c.im_ << "i";
        return os;
    }
};

} // namespace types

#endif // TYPES_COMPLEX_HPP
