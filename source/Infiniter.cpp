#include "Infiniter.hpp"



Infiniter::Infiniter() noexcept
    : InfiniterUtility()
{

}

Infiniter::Infiniter(isize_t p_capacity)
    : InfiniterUtility(p_capacity)
{

}

Infiniter::Infiniter(isize_t p_capacity, icell_t p_value, bool p_negative_value)
    : InfiniterUtility(p_capacity, p_value, p_negative_value)
{

}

Infiniter::Infiniter(const icell_t *p_array, isize_t p_size, bool p_negative_value)
    : InfiniterUtility(p_array, p_size, p_negative_value)
{

}

Infiniter::Infiniter(const std::string &p_number, int p_base, bool p_negative_value)
    : InfiniterUtility(p_number, p_base, p_negative_value)
{

}

Infiniter::Infiniter(const std::vector<uint8_t> &p_number, int p_base, bool p_negative_value)
    : InfiniterUtility(p_number, p_base, p_negative_value)
{

}

Infiniter::Infiniter(const Infiniter &p_source)
    : InfiniterUtility(p_source)
{

}

Infiniter::Infiniter(Infiniter &&p_source) noexcept
    : InfiniterUtility(std::move(p_source))
{

}

Infiniter::Infiniter(const InfiniterUtility &p_source)
    : InfiniterUtility(p_source)
{

}

Infiniter::Infiniter(InfiniterUtility &&p_source) noexcept
    : InfiniterUtility(std::move(p_source))
{

}

Infiniter::Infiniter(const InfiniterIO &p_source)
    : InfiniterUtility(p_source)
{

}

Infiniter::Infiniter(InfiniterIO &&p_source) noexcept
    : InfiniterUtility(std::move(p_source))
{

}

Infiniter::Infiniter(const InfiniterArithmetic &p_source)
    : InfiniterUtility(p_source)
{

}

Infiniter::Infiniter(InfiniterArithmetic &&p_source) noexcept
    : InfiniterUtility(std::move(p_source))
{

}

Infiniter::Infiniter(const InfiniterBit &p_source)
    : InfiniterUtility(p_source)
{

}

Infiniter::Infiniter(InfiniterBit &&p_source) noexcept
    : InfiniterUtility(std::move(p_source))
{

}

Infiniter::Infiniter(const InfiniterCore &p_source)
    : InfiniterUtility(p_source)
{

}

Infiniter::Infiniter(InfiniterCore &&p_source) noexcept
    : InfiniterUtility(std::move(p_source))
{

}

void Infiniter::assign(const Infiniter &p_source)
{
    InfiniterIO::assign(p_source);
}

void Infiniter::assign(Infiniter &&p_source)
{
    InfiniterIO::assign(std::move(p_source));
}

Infiniter &Infiniter::operator =(const Infiniter &p_source)
{
    InfiniterIO::assign(p_source);
    return *this;
}

Infiniter &Infiniter::operator =(Infiniter &&p_source)
{
    InfiniterIO::assign(std::move(p_source));
    return *this;
}
