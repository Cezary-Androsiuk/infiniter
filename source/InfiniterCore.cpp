#include "InfiniterCore.h"

#include "InfiniterCommon.hpp"

#include <utility> // std::move

/// Constructed     TYPE    %p
/// Assigned        TYPE    %p
/// DEL                     %p
/// ic_dbgprintf("IC Constructed   DEFAULT       %p\n", this);
/// ic_dbgprintf("IC Constructed   PARAMETER     %p\n", this);
/// ic_dbgprintf("IC Constructed   COPY          %p\n", this);
/// ic_dbgprintf("IC Constructed   MOVE          %p\n", this);
/// ic_dbgprintf("IC DEL                         %p\n", this);
/// ic_dbgprintf("IC Assigned      COPY          %p\n", this);
/// ic_dbgprintf("IC Assigned      MOVE          %p\n", this);
#if INFINITER_CORE_DEBUG_PRINT
#include <cstdio>
#define ic_dbgprintf(...) printf(__VA_ARGS__);
#else
#define ic_dbgprintf(...)
#endif

InfiniterCore::InfiniterCore() noexcept
    : InfiniterMemory()
{
    ic_dbgprintf("IC Constructed   DEFAULT       %p\n", this);
    m_bits.sign = false;
}

InfiniterCore::InfiniterCore(uint64_t p_capacity)
    : InfiniterMemory(p_capacity) /// ensures that final capacity will be grater or equal to SBO_CAPACITY
{
    ic_dbgprintf("IC Constructed   PARAMETER 1   %p\n", this);
    m_bits.sign = false;
}

InfiniterCore::InfiniterCore(uint64_t p_capacity, uint64_t p_value, bool p_negative_value)
    : InfiniterMemory(p_capacity) /// ensures that final capacity will be grater or equal to SBO_CAPACITY
{
    ic_dbgprintf("IC Constructed   PARAMETER 2   %p\n", this);

    m_bits.sign = p_negative_value;
    // assign value
    m_memory[0] = p_value;

#if CLEAR_ALLOCATED_MEMORY

#endif

/// that wil be interesing idea to handle msb if sbo_capacity is 1, but it is not necessary
// #if SBO_CAPACITY > 1
//     m_memory[0] = p_value;
// #else
// #endif


}

InfiniterCore::InfiniterCore(const InfiniterCore &p_source)
    : InfiniterMemory(p_source)
{
    ic_dbgprintf("IC Constructed   COPY          %p\n", this);
    // copy value
}

InfiniterCore::InfiniterCore(InfiniterCore &&p_source) noexcept
    : InfiniterMemory(std::move(p_source))
{
    ic_dbgprintf("IC Constructed   MOVE          %p\n", this);
    // move value, or just sign
}

InfiniterCore &InfiniterCore::operator =(const InfiniterCore &p_source)
{
    ic_dbgprintf("IC Assigned      COPY          %p\n", this);

    if( &p_source != this )
    {
        InfiniterMemory::operator=(p_source);



    }

    return *this;

}

InfiniterCore &InfiniterCore::operator =(InfiniterCore &&p_source)
{
    ic_dbgprintf("IC Assigned      MOVE          %p\n", this);

    if( &p_source != this )
    {
        InfiniterMemory::operator=(std::move(p_source));


    }

    return *this;

}


