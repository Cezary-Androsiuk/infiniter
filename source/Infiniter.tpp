#pragma once

#include "Infiniter.hpp"

Infiniter::Infiniter() noexcept
    : InfiniterUtility<Infiniter>()
{

}

Infiniter::Infiniter(isize_t p_capacity)
    : InfiniterUtility<Infiniter>(p_capacity)
{

}

Infiniter::Infiniter(isize_t p_capacity, icell_t p_value, bool p_negative_value)
    : InfiniterUtility<Infiniter>(p_capacity, p_value, p_negative_value)
{

}

Infiniter::Infiniter(const icell_t *p_array, isize_t p_size, bool p_negative_value)
    : InfiniterUtility<Infiniter>(p_array, p_size, p_negative_value)
{

}

Infiniter::Infiniter(const std::string &p_number, int p_base, bool p_negative_value)
    : InfiniterUtility<Infiniter>(p_number, p_base, p_negative_value)
{

}

Infiniter::Infiniter(const std::vector<uint8_t> &p_number, int p_base, bool p_negative_value)
    : InfiniterUtility<Infiniter>(p_number, p_base, p_negative_value)
{

}

Infiniter::Infiniter(const Infiniter &p_source)
    : InfiniterUtility<Infiniter>(p_source)
{

}

Infiniter::Infiniter(Infiniter &&p_source) noexcept
    : InfiniterUtility<Infiniter>(std::move(p_source))
{

}

inline Infiniter::~Infiniter() noexcept
{

}

Infiniter &Infiniter::operator =(const Infiniter &p_source)
{
    return InfiniterCore<Infiniter>::operator=(p_source);
}

Infiniter &Infiniter::operator =(Infiniter &&p_source)
{
    return InfiniterCore<Infiniter>::operator=(std::move(p_source));
}
