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
    m_size = 1ull;
}

InfiniterCore::InfiniterCore(uint64_t p_capacity)
    : InfiniterMemory(p_capacity) /// ensures that final capacity will be grater or equal to SBO_CAPACITY
{
    ic_dbgprintf("IC Constructed   PARAMETER 1   %p\n", this);

    m_bits.sign = false;
    m_size = 1ull;
}

InfiniterCore::InfiniterCore(uint64_t p_capacity, uint64_t p_value, bool p_negative_value)
    : InfiniterMemory(p_capacity) /// ensures that final capacity will be grater or equal to SBO_CAPACITY
{
    ic_dbgprintf("IC Constructed   PARAMETER 2   %p\n", this);

    m_bits.sign = p_negative_value;
    m_size = 1ull;

    /// assign value
    m_memory[0] = p_value;

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

    /// entire memory was copied in IM

    m_bits.sign = p_source.m_bits.sign;
    m_size = p_source.m_size;
}

InfiniterCore::InfiniterCore(InfiniterCore &&p_source) noexcept
    : InfiniterMemory(std::move(p_source))
{
    ic_dbgprintf("IC Constructed   MOVE          %p\n", this);

    /// entire memory was moved in IM

    m_bits.sign = p_source.m_bits.sign;
    m_size = p_source.m_size;
}

InfiniterCore::~InfiniterCore() noexcept
{
    /// everything was done in InfiniterMemory
}

void InfiniterCore::reset() noexcept
{
    InfiniterMemory::reset(); /// makes m_capacity == SBO_CAPACITY
    m_bits.sign = false;

    for(int i=0; i<m_capacity; i++)
    {
        m_memory[i] = 0;
    }
}

void InfiniterCore::reserve(uint64_t p_new_capacity)
{
    InfiniterMemory::reserve(p_new_capacity);
}

void InfiniterCore::reserve(const InfiniterCore &p_source)
{
    InfiniterMemory::reserve(p_source);
}

void InfiniterCore::extend(uint64_t p_additional_capacity)
{
    InfiniterMemory::extend(p_additional_capacity);
}

void InfiniterCore::shrink()
{
    /// shrink to m_size
    InfiniterMemory::shrink(m_size);
}

cell_t *InfiniterCore::getData() noexcept
{
    return m_memory;
}

const cell_t *InfiniterCore::getData() const noexcept
{
    return m_memory;
}

uint64_t InfiniterCore::getSize() const noexcept
{
    return m_size;
}

uint64_t InfiniterCore::getCapacity() const noexcept
{
    return m_capacity;
}

uint8_t InfiniterCore::getSign() const noexcept
{
    return m_bits.sign;
}

InfiniterCore &InfiniterCore::operator =(const InfiniterCore &p_source)
{
    ic_dbgprintf("IC Assigned      COPY          %p\n", this);

    if( &p_source != this )
    {
        InfiniterMemory::operator=(p_source);

        /// entire memory was copied in IM

        m_bits.sign = p_source.m_bits.sign;
        m_size = p_source.m_size;
    }

    return *this;

}

InfiniterCore &InfiniterCore::operator =(InfiniterCore &&p_source)
{
    ic_dbgprintf("IC Assigned      MOVE          %p\n", this);

    if( &p_source != this )
    {
        InfiniterMemory::operator=(std::move(p_source));

        /// entire memory was moved in IM

        m_bits.sign = p_source.m_bits.sign;
        m_size = p_source.m_size;
    }

    return *this;

}


