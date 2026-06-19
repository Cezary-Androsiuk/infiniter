#pragma once

#include "Infiniter.hpp"

/// _i_dbgprintf("--- DEBUG I %p | Constructed   DEFAULT\n", this);
/// _i_dbgprintf("--- DEBUG I %p | Constructed   PARAMETER\n", this);
/// _i_dbgprintf("--- DEBUG I %p | Constructed   COPY\n", this);
/// _i_dbgprintf("--- DEBUG I %p | Constructed   MOVE\n", this);
/// _i_dbgprintf("--- DEBUG I %p | Assigned      COPY\n", this);
/// _i_dbgprintf("--- DEBUG I %p | Assigned      MOVE\n", this);
/// _i_dbgprintf("--- DEBUG I %p | Delete\n", this);
/// _i_dbgprintf("--- DEBUG I %p | Other         (...)\n", this);
#if I_DEBUG_EXECUTION_PRINT
#include <cstdio>
#define _i_dbgprintf(...) printf(__VA_ARGS__);
#else // I_DEBUG_EXECUTION_PRINT
#define _i_dbgprintf(...)
#endif // I_DEBUG_EXECUTION_PRINT

Infiniter::Infiniter() noexcept
    : InfiniterUtility<Infiniter>()
{
    _i_dbgprintf("--- DEBUG I %p | Constructed   DEFAULT\n", this);
}

Infiniter::Infiniter(int64_t p_value)
    : InfiniterUtility<Infiniter>(p_value)
{
    _i_dbgprintf("--- DEBUG I %p | Constructed   PARAMETER int64_t\n", this);
}

Infiniter::Infiniter(int64_t p_value, isize_t p_capacity)
    : InfiniterUtility<Infiniter>(p_value, p_capacity)
{
    _i_dbgprintf("--- DEBUG I %p | Constructed   PARAMETER int64_t isize_t\n", this);
}

Infiniter::Infiniter(icell_t p_value, isize_t p_capacity, bool p_negative_value)
    : InfiniterUtility<Infiniter>(p_value, p_capacity, p_negative_value)
{
    _i_dbgprintf("--- DEBUG I %p | Constructed   PARAMETER icell_t isize_t bool\n", this);
}

Infiniter::Infiniter(const icell_t *p_array, isize_t p_size, bool p_negative_value)
    : InfiniterUtility<Infiniter>(p_array, p_size, p_negative_value)
{
    _i_dbgprintf("--- DEBUG I %p | Constructed   PARAMETER icell_t* isize_t bool\n", this);
}

Infiniter::Infiniter(const std::string &p_number, int p_base, bool p_negative_value)
    : InfiniterUtility<Infiniter>(p_number, p_base, p_negative_value)
{
    _i_dbgprintf("--- DEBUG I %p | Constructed   PARAMETER std::string int bool\n", this);
}

Infiniter::Infiniter(const std::vector<uint8_t> &p_number, int p_base, bool p_negative_value)
    : InfiniterUtility<Infiniter>(p_number, p_base, p_negative_value)
{
    _i_dbgprintf("--- DEBUG I %p | Constructed   PARAMETER std::vector<uint8_t> int bool\n", this);
}

Infiniter::Infiniter(const Infiniter &p_source)
    : InfiniterUtility<Infiniter>(p_source)
{
    _i_dbgprintf("--- DEBUG I %p | Constructed   COPY\n", this);
}

Infiniter::Infiniter(Infiniter &&p_source) noexcept
    : InfiniterUtility<Infiniter>(std::move(p_source))
{
    _i_dbgprintf("--- DEBUG I %p | Constructed   MOVE\n", this);
}

inline Infiniter::~Infiniter() noexcept
{
    _i_dbgprintf("--- DEBUG I %p | Delete\n", this);
}

Infiniter &Infiniter::operator =(const Infiniter &p_source)
{
    _i_dbgprintf("--- DEBUG I %p | Assigned OPE. COPY\n", this);
    return InfiniterCore<Infiniter>::operator=(p_source);
}

Infiniter &Infiniter::operator =(Infiniter &&p_source)
{
    _i_dbgprintf("--- DEBUG I %p | Assigned OPE. MOVE\n", this);
    return InfiniterCore<Infiniter>::operator=(std::move(p_source));
}
