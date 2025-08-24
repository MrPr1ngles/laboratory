#pragma once

template <typename T>
class Function {
public:
    typedef T (*FuncPtr)(T);

    Function(FuncPtr f = nullptr) : func(f) {}

    T operator()(T input) const {
        return func(input);
    }

    bool operator==(const Function& other) const {
        return func == other.func;
    }

private:
    FuncPtr func;
};