#ifndef INFINITEREXCEPTION_HPP
#define INFINITEREXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace InfiniterException{
    class Exception : public std::runtime_error { public:
        explicit Exception(const std::string &p_message)
            : std::runtime_error(p_message) {}
    };

    class InvalidStringFormat : public Exception { public:
        explicit InvalidStringFormat(const std::string &p_message)
            : Exception("Invalid String Format: " + p_message) {}
    };

    class InvalidVectorFormat : public Exception { public:
        explicit InvalidVectorFormat(const std::string &p_message)
            : Exception("Invalid Vector Format: " + p_message) {}
    };

    class InvalidBase : public Exception { public:
        explicit InvalidBase(uint8_t p_base, uint8_t p_min_base, uint8_t p_max_base)
            : Exception(
                  "Invalid base " + std::to_string(p_base) +
                  ", must be between " + std::to_string(p_min_base) +
                  " and " + std::to_string(p_min_base)) {}
    };

    class InputContainerCapacityExceeded : public Exception { public:
        explicit InputContainerCapacityExceeded(uint64_t p_input_size, uint64_t p_max_size)
            : Exception(
                  "Input Container Capacity Exceeded: "
                  "Max capacity is " + std::to_string(p_max_size) +
                  " and got container with size " + std::to_string(p_input_size)) {}
    };

    class CapacityApproximationFailed : public Exception { public:
        explicit CapacityApproximationFailed(const std::string &p_message)
            : Exception("Capacity Approximation Failed: " + p_message) {}
    };
}


#endif // INFINITEREXCEPTION_HPP
