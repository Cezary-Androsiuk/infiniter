#ifndef INFINITER_HPP
#define INFINITER_HPP

#include "InfiniterShared.hpp"
#include "InfiniterUtility.hpp"

#include <string>
#include <vector>

class Infiniter : public InfiniterUtility
{
public:
    explicit Infiniter() noexcept;

    explicit Infiniter(isize_t p_capacity); /// throws bad_alloc
    explicit Infiniter(isize_t p_capacity, icell_t p_value, bool p_negative_value=false); /// throws bad_alloc
    explicit Infiniter(const icell_t *p_array, isize_t p_size, bool p_negative_value=false); /// throws bad_alloc

    explicit Infiniter(const std::string &p_number, int p_base=10, bool p_negative_value=false); /// throws bad_alloc /// InfiniterException::InvalidStringFormat
    explicit Infiniter(const std::vector<uint8_t> &p_number, int p_base=10, bool p_negative_value=false); /// throws bad_alloc

    Infiniter(const Infiniter &p_source); /// throws bad_alloc
    Infiniter(Infiniter &&p_source) noexcept;

    Infiniter(const InfiniterUtility &p_source); /// throws bad_alloc
    Infiniter(InfiniterUtility &&p_source) noexcept;

    Infiniter(const InfiniterIO &p_source); /// throws bad_alloc
    Infiniter(InfiniterIO &&p_source) noexcept;

    Infiniter(const InfiniterArithmetic &p_source); /// throws bad_alloc
    Infiniter(InfiniterArithmetic &&p_source) noexcept;

    Infiniter(const InfiniterBit &p_source); /// throws bad_alloc
    Infiniter(InfiniterBit &&p_source) noexcept;

    Infiniter(const InfiniterCore &p_source); /// throws bad_alloc
    Infiniter(InfiniterCore &&p_source) noexcept;




    /// InfiniterIO methods


    using InfiniterUtility::assign;
    void assign(const Infiniter &p_source);
    void assign(Infiniter &&p_source);

public:

    /// Operators
    using InfiniterUtility::operator = ;
    Infiniter &operator = (const Infiniter &p_source);
    Infiniter &operator = (Infiniter &&p_source);
};

#endif // INFINITER_HPP
