#include "InfiniterArithmetic.hpp"

#include "InfiniterShared.hpp"

#include "Infiniter.hpp"

#include <utility> // std::move

/// _ia_dbgprintf("--- DEBUG IA %p | Constructed   DEFAULT\n", this);
/// _ia_dbgprintf("--- DEBUG IA %p | Constructed   PARAMETER\n", this);
/// _ia_dbgprintf("--- DEBUG IA %p | Constructed   COPY\n", this);
/// _ia_dbgprintf("--- DEBUG IA %p | Constructed   MOVE\n", this);
/// _ia_dbgprintf("--- DEBUG IA %p | Assigned      COPY\n", this);
/// _ia_dbgprintf("--- DEBUG IA %p | Assigned      MOVE\n", this);
/// _ia_dbgprintf("--- DEBUG IA %p | Delete\n", this);
/// _ia_dbgprintf("--- DEBUG IA %p | Other         (...)\n", this);
#if IA_DEBUG_EXECUTION_PRINT
#include <cstdio>
#define _ia_dbgprintf(...) printf(__VA_ARGS__);
#else // IA_DEBUG_EXECUTION_PRINT
#define _ia_dbgprintf(...)
#endif // IA_DEBUG_EXECUTION_PRINT

InfiniterArithmetic::InfiniterArithmetic() noexcept
    : InfiniterBit()
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed   DEFAULT\n", this);
}

InfiniterArithmetic::InfiniterArithmetic(uint64_t p_capacity)
    : InfiniterBit(p_capacity)
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed   PARAMETER uint64_t\n", this);
}

InfiniterArithmetic::InfiniterArithmetic(uint64_t p_capacity, uint64_t p_value, bool p_negative_value)
    : InfiniterBit(p_capacity, p_value, p_negative_value)
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed   PARAMETER uint64_t uint64_t bool\n", this);
}

InfiniterArithmetic::InfiniterArithmetic(const cell_t *p_array, uint64_t p_size, bool p_negative_value)
    : InfiniterBit(p_array, p_size, p_negative_value)
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed   PARAMETER cell_t* uint64_t bool\n", this);
}

InfiniterArithmetic::InfiniterArithmetic(const InfiniterArithmetic &p_source)
    : InfiniterBit(p_source)
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed   COPY\n", this);
}

InfiniterArithmetic::InfiniterArithmetic(InfiniterArithmetic &&p_source) noexcept
    : InfiniterBit(std::move(p_source))
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed   MOVE\n", this);
}

InfiniterArithmetic::~InfiniterArithmetic() noexcept
{
    _ia_dbgprintf("--- DEBUG IA %p | Delete\n", this);

    /// everything was done in InfiniterMemory
}
