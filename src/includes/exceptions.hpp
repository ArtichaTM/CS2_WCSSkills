#ifndef WCSSKILLS_EXCEPTIONS_HPP
#define WCSSKILLS_EXCEPTIONS_HPP

#include <exception>
#include <string>

class CustomException : public std::exception {
public:
    explicit CustomException(char[]);
    explicit CustomException(const char[]);
    explicit CustomException();
    const char* message;
    [[nodiscard]] char const* what() const noexcept;
};


#endif //WCSSKILLS_EXCEPTIONS_HPP
