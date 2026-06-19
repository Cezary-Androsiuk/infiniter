#pragma once

#include "InfiniterShared.hpp"
#include "InfiniterUtility.hpp"

#include <string>
#include <vector>

class Infiniter : public InfiniterUtility<Infiniter>
{
public:
    inline explicit Infiniter() noexcept;

    inline Infiniter(int64_t p_value); /// throws bad_alloc
    inline explicit Infiniter(int64_t p_value, isize_t p_capacity); /// throws bad_alloc
    inline explicit Infiniter(icell_t p_value, isize_t p_capacity, bool p_negative_value); /// throws bad_alloc
    inline explicit Infiniter(const icell_t *p_array, isize_t p_size, bool p_negative_value=false); /// throws bad_alloc

    inline explicit Infiniter(const std::string &p_number, int p_base=10, bool p_negative_value=false); /// throws bad_alloc /// InfiniterException::InvalidStringFormat
    inline explicit Infiniter(const std::vector<uint8_t> &p_number, int p_base=10, bool p_negative_value=false); /// throws bad_alloc

    inline Infiniter(const Infiniter &p_source); /// throws bad_alloc
    inline Infiniter(Infiniter &&p_source) noexcept;

    inline ~Infiniter() noexcept;

    /// InfiniterUtility methods

public:

    /// Operators
    Infiniter &operator = (const Infiniter &p_source);
    Infiniter &operator = (Infiniter &&p_source);

};

#include "Infiniter.tpp"
