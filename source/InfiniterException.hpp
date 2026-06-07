#ifndef INFINITEREXCEPTION_HPP
#define INFINITEREXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace InfiniterException{
    class Exception : public std::runtime_error
    {
    public:
        explicit Exception(const std::string &message)
            : std::runtime_error(message) {}
    };

    class InvalidStringFormat : public Exception
    {
    public:
        explicit InvalidStringFormat(const std::string &message)
            : Exception("Invalid String Format: " + message) {}
    };
}


#endif // INFINITEREXCEPTION_HPP
