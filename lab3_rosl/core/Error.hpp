#ifndef CORE_ERROR_HPP
#define CORE_ERROR_HPP

#include <iostream>
#include <cstdlib>

namespace core {

enum class ErrorCode {
    NONE = 0,
    EMPTY_CONTAINER,
    INDEX_OUT_OF_RANGE,
    INVALID_ARGUMENT,
    ALLOCATION_ERROR,
    NOT_FOUND,
    SUBSEQUENCE_NOT_FOUND,
    TYPE_ERROR,
    IO_ERROR,
    MERGE_ERROR,
    SORT_ERROR
};

inline const char* error_code_to_string(ErrorCode e) {
    switch(e) {
        case ErrorCode::NONE: return "No error";
        case ErrorCode::EMPTY_CONTAINER: return "Empty container";
        case ErrorCode::INDEX_OUT_OF_RANGE: return "Index out of range";
        case ErrorCode::INVALID_ARGUMENT: return "Invalid argument";
        case ErrorCode::ALLOCATION_ERROR: return "Allocation error";
        case ErrorCode::NOT_FOUND: return "Not found";
        case ErrorCode::SUBSEQUENCE_NOT_FOUND: return "Subsequence not found";
        case ErrorCode::TYPE_ERROR: return "Type error";
        case ErrorCode::IO_ERROR: return "I/O error";
        case ErrorCode::MERGE_ERROR: return "Merge error";
        case ErrorCode::SORT_ERROR: return "Sort error";
        default: return "Unknown error";
    }
}

// Базовая стратегия: логировать в stderr и завершать программу при критической ошибке.
// В тестах можно модифицировать это поведение.
inline void abort_with_error(ErrorCode code, const char* info = nullptr) {
    std::cerr << "ERROR [" << static_cast<int>(code) << "]: " << error_code_to_string(code);
    if (info) std::cerr << " (" << info << ")";
    std::cerr << std::endl;
    std::exit(static_cast<int>(code));
}

} // namespace core

#endif // CORE_ERROR_HPP
