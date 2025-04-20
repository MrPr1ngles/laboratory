#ifndef ERROR_HPP
#define ERROR_HPP

enum class ErrorCode {
    OK = 0,
    INVALID_INPUT = 1,
    INDEX_OUT_OF_RANGE = 2
};

struct Error {
    int code;
    char message[64];
};

static const Error Errors[] = {
    {0, "Success"},
    {1, "Invalid input"},
    {2, "Index out of range"}
};

static const int ERRORS_COUNT = sizeof(Errors) / sizeof(Errors[0]);

inline const char* GetErrorMessage(int code) {
    for (int i = 0; i < ERRORS_COUNT; i++) {
        if (Errors[i].code == code)
            return Errors[i].message;
    }
    return "Unknown error";
}

#endif
