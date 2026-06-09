#include "InfiniterUtility.hpp"

#include "InfiniterShared.hpp"

#include "Infiniter.hpp"

#include <utility> // std::move

/// _iu_dbgprintf("--- DEBUG IU %p | Constructed   DEFAULT\n", this);
/// _iu_dbgprintf("--- DEBUG IU %p | Constructed   PARAMETER\n", this);
/// _iu_dbgprintf("--- DEBUG IU %p | Constructed   COPY\n", this);
/// _iu_dbgprintf("--- DEBUG IU %p | Constructed   MOVE\n", this);
/// _iu_dbgprintf("--- DEBUG IU %p | Assigned      COPY\n", this);
/// _iu_dbgprintf("--- DEBUG IU %p | Assigned      MOVE\n", this);
/// _iu_dbgprintf("--- DEBUG IU %p | Delete\n", this);
/// _iu_dbgprintf("--- DEBUG IU %p | Other         (...)\n", this);
#if IU_DEBUG_EXECUTION_PRINT
#include <cstdio>
#define _iu_dbgprintf(...) printf(__VA_ARGS__);
#else // IU_DEBUG_EXECUTION_PRINT
#define _iu_dbgprintf(...)
#endif // IU_DEBUG_EXECUTION_PRINT

InfiniterUtility::InfiniterUtility() noexcept
    : InfiniterIO()
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed   DEFAULT\n", this);
}

InfiniterUtility::InfiniterUtility(isize_t p_capacity)
    : InfiniterIO(p_capacity)
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed   PARAMETER isize_t\n", this);
}

InfiniterUtility::InfiniterUtility(isize_t p_capacity, icell_t p_value, bool p_negative_value)
    : InfiniterIO(p_capacity, p_value, p_negative_value)
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed   PARAMETER isize_t icell_t bool\n", this);
}

InfiniterUtility::InfiniterUtility(const icell_t *p_array, isize_t p_size, bool p_negative_value)
    : InfiniterIO(p_array, p_size, p_negative_value)
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed   PARAMETER icell_t* isize_t bool\n", this);
}

InfiniterUtility::InfiniterUtility(const std::string &p_number, int p_base, bool p_negative_value)
    : InfiniterIO(p_number, p_base, p_negative_value)
{
    _iu_dbgprintf("--- DEBUG IO %p | Constructed   PARAMETER std::string int bool\n", this);
}

InfiniterUtility::InfiniterUtility(const std::vector<uint8_t> &p_number, int p_base, bool p_negative_value)
    : InfiniterIO(p_number, p_base, p_negative_value)
{
    _iu_dbgprintf("--- DEBUG IO %p | Constructed   PARAMETER std::vector<uint8_t> int bool\n", this);
}

InfiniterUtility::InfiniterUtility(const InfiniterUtility &p_source)
    : InfiniterIO(p_source)
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed IU COPY\n", this);
}

InfiniterUtility::InfiniterUtility(InfiniterUtility &&p_source) noexcept
    : InfiniterIO(std::move(p_source))
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed IU MOVE\n", this);
}

InfiniterUtility::InfiniterUtility(const InfiniterIO &p_source)
    : InfiniterIO(p_source)
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed IO COPY\n", this);
}

InfiniterUtility::InfiniterUtility(InfiniterIO &&p_source) noexcept
    : InfiniterIO(std::move(p_source))
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed IO MOVE\n", this);
}

InfiniterUtility::InfiniterUtility(const InfiniterArithmetic &p_source)
    : InfiniterIO(p_source)
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed IA COPY\n", this);
}

InfiniterUtility::InfiniterUtility(InfiniterArithmetic &&p_source) noexcept
    : InfiniterIO(std::move(p_source))
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed IA MOVE\n", this);
}

InfiniterUtility::InfiniterUtility(const InfiniterBit &p_source)
    : InfiniterIO(p_source)
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed IB COPY\n", this);
}

InfiniterUtility::InfiniterUtility(InfiniterBit &&p_source) noexcept
    : InfiniterIO(std::move(p_source))
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed IB MOVE\n", this);
}

InfiniterUtility::InfiniterUtility(const InfiniterCore &p_source)
    : InfiniterIO(p_source)
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed IC COPY\n", this);
}

InfiniterUtility::InfiniterUtility(InfiniterCore &&p_source) noexcept
    : InfiniterIO(std::move(p_source))
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed IC MOVE\n", this);
}

InfiniterUtility::~InfiniterUtility() noexcept
{
    _iu_dbgprintf("--- DEBUG IU %p | Delete\n", this);

    /// everything was done in InfiniterMemory
}

void InfiniterUtility::assign(const InfiniterUtility &p_source)
{
    InfiniterIO::assign(p_source);
}

void InfiniterUtility::assign(InfiniterUtility &&p_source)
{
    InfiniterIO::assign(std::move(p_source));
}

InfiniterUtility &InfiniterUtility::operator =(const InfiniterUtility &p_source)
{
    InfiniterIO::assign(p_source);
    return *this;
}

InfiniterUtility &InfiniterUtility::operator =(InfiniterUtility &&p_source)
{
    InfiniterIO::assign(std::move(p_source));
    return *this;
}


