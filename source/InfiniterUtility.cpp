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
    : InfiniterArithmetic()
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed   DEFAULT\n", this);
}

InfiniterUtility::InfiniterUtility(uint64_t p_capacity)
    : InfiniterArithmetic(p_capacity)
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed   PARAMETER uint64_t\n", this);
}

InfiniterUtility::InfiniterUtility(uint64_t p_capacity, uint64_t p_value, bool p_negative_value)
    : InfiniterArithmetic(p_capacity, p_value, p_negative_value)
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed   PARAMETER uint64_t uint64_t bool\n", this);
}

InfiniterUtility::InfiniterUtility(const cell_t *p_array, uint64_t p_size, bool p_negative_value)
    : InfiniterArithmetic(p_array, p_size, p_negative_value)
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed   PARAMETER cell_t* uint64_t bool\n", this);
}

InfiniterUtility::InfiniterUtility(const InfiniterUtility &p_source)
    : InfiniterArithmetic(p_source)
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed   COPY\n", this);
}

InfiniterUtility::InfiniterUtility(InfiniterUtility &&p_source) noexcept
    : InfiniterArithmetic(std::move(p_source))
{
    _iu_dbgprintf("--- DEBUG IU %p | Constructed   MOVE\n", this);
}

InfiniterUtility::~InfiniterUtility() noexcept
{
    _iu_dbgprintf("--- DEBUG IU %p | Delete\n", this);

    /// everything was done in InfiniterMemory
}
