#pragma once

#include "InfiniterUtility.hpp"

#include "InfiniterShared.hpp"

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

template<typename InfiniterDerived>
InfiniterUtility<InfiniterDerived>::InfiniterUtility() noexcept
    : InfiniterIO<InfiniterDerived>()
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed   DEFAULT\n", this);
}

template<typename InfiniterDerived>
InfiniterUtility<InfiniterDerived>::InfiniterUtility(isize_t p_capacity)
    : InfiniterIO<InfiniterDerived>(p_capacity)
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed   PARAMETER isize_t\n", this);
}

template<typename InfiniterDerived>
InfiniterUtility<InfiniterDerived>::InfiniterUtility(isize_t p_capacity, icell_t p_value, bool p_negative_value)
    : InfiniterIO<InfiniterDerived>(p_capacity, p_value, p_negative_value)
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed   PARAMETER isize_t icell_t bool\n", this);
}

template<typename InfiniterDerived>
InfiniterUtility<InfiniterDerived>::InfiniterUtility(const icell_t *p_array, isize_t p_size, bool p_negative_value)
    : InfiniterIO<InfiniterDerived>(p_array, p_size, p_negative_value)
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed   PARAMETER icell_t* isize_t bool\n", this);
}

template<typename InfiniterDerived>
InfiniterUtility<InfiniterDerived>::InfiniterUtility(const std::string &p_number, int p_base, bool p_negative_value)
    : InfiniterIO<InfiniterDerived>(p_number, p_base, p_negative_value)
{
    _iu_dbgprintf("--- DEBUG IO %p | Constructed   PARAMETER std::string int bool\n", this);
}

template<typename InfiniterDerived>
InfiniterUtility<InfiniterDerived>::InfiniterUtility(const std::vector<uint8_t> &p_number, int p_base, bool p_negative_value)
    : InfiniterIO<InfiniterDerived>(p_number, p_base, p_negative_value)
{
    _iu_dbgprintf("--- DEBUG IO %p | Constructed   PARAMETER std::vector<uint8_t> int bool\n", this);
}

template<typename InfiniterDerived>
InfiniterUtility<InfiniterDerived>::InfiniterUtility(const InfiniterDerived &p_source)
    : InfiniterIO<InfiniterDerived>(p_source)
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed IU COPY\n", this);
}

template<typename InfiniterDerived>
InfiniterUtility<InfiniterDerived>::InfiniterUtility(InfiniterDerived &&p_source) noexcept
    : InfiniterIO<InfiniterDerived>(std::move(p_source))
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed IU MOVE\n", this);
}

template<typename InfiniterDerived>
InfiniterUtility<InfiniterDerived>::~InfiniterUtility() noexcept
{
    _iu_dbgprintf("--- DEBUG IU %p | Delete\n", this);

    /// everything was done in InfiniterMemory
}

template<typename InfiniterDerived>
inline InfiniterDerived InfiniterUtility<InfiniterDerived>::findGCD(const InfiniterDerived &p_number) const
{

    // return std::move();
}

template<typename InfiniterDerived>
inline InfiniterDerived InfiniterUtility<InfiniterDerived>::findLCM(const InfiniterDerived &p_number) const
{

    // return std::move();
}

template<typename InfiniterDerived>
inline bool InfiniterUtility<InfiniterDerived>::isPrime() const
{

}
