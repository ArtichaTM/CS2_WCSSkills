#include "exceptions.hpp"

[[nodiscard]] char const* CustomException::what() const noexcept { return message; }

CustomException::CustomException(char _message[]) : message(_message), std::exception() {}
CustomException::CustomException(const char _message[]) : message(_message), std::exception() {}
CustomException::CustomException() : message((char*) ""), std::exception() {}
