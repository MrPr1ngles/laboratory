#ifndef ERROR_HPP
#define ERROR_HPP

enum ErrorCode {
    OK = 0,
    INVALID_INPUT = 1,
    INDEX_OUT_OF_RANGE = 2,
    RUNTIME_ERROR = 3
};

struct Error {
    int         code;
    const char* message;
};

static const Error Errors[] = {
    { OK,                "Success" },
    { INVALID_INPUT,     "Invalid input" },
    { INDEX_OUT_OF_RANGE,"Index out of range" },
    { RUNTIME_ERROR,     "RUNTIME_ERROR"}
};

inline const char* GetErrorMessage(int code) {
    for (const auto& e : Errors) {
        if (e.code == code) return e.message;
    }
    return "Unknown error";
}

#endif 
