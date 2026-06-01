#include "InfiniterCore.h"

#include "InfiniterShared.hpp"

#include <utility> // std::move

/// Constructed     TYPE    %p
/// Assigned        TYPE    %p
/// DEL                     %p
/// _ic_dbgprintf("IC Constructed   DEFAULT       %p\n", this);
/// _ic_dbgprintf("IC Constructed   PARAMETER     %p\n", this);
/// _ic_dbgprintf("IC Constructed   COPY          %p\n", this);
/// _ic_dbgprintf("IC Constructed   MOVE          %p\n", this);
/// _ic_dbgprintf("IC DEL                         %p\n", this);
/// _ic_dbgprintf("IC Assigned      COPY          %p\n", this);
/// _ic_dbgprintf("IC Assigned      MOVE          %p\n", this);
#if IC_DEBUG_EXECUTION_PRINT
#include <cstdio>
#define _ic_dbgprintf(...) printf(__VA_ARGS__);
#else // IC_DEBUG_EXECUTION_PRINT
#define _ic_dbgprintf(...)
#endif // IC_DEBUG_EXECUTION_PRINT

InfiniterCore::InfiniterCore() noexcept
    : InfiniterMemory()
{
    _ic_dbgprintf("IC Constructed   DEFAULT       %p\n", this);

    m_bits.sign = false;
    m_size = 1ull;
}

InfiniterCore::InfiniterCore(uint64_t p_capacity)
    : InfiniterMemory(p_capacity) /// ensures that final capacity will be grater or equal to SBO_CAPACITY
{
    _ic_dbgprintf("IC Constructed   PARAMETER 1   %p\n", this);

    m_bits.sign = false;
    m_size = 1ull;
}

InfiniterCore::InfiniterCore(uint64_t p_capacity, uint64_t p_value, bool p_negative_value)
    : InfiniterMemory(p_capacity) /// ensures that final capacity will be grater or equal to SBO_CAPACITY
{
    _ic_dbgprintf("IC Constructed   PARAMETER 2   %p\n", this);

    m_bits.sign = p_negative_value;
    m_size = 1ull;

    /// assign value
    m_memory[0] = p_value;
}

InfiniterCore::InfiniterCore(const cell_t *p_array, uint64_t p_size, bool p_negative_value)
    : InfiniterMemory(p_size) /// ensures that final capacity will be grater or equal to SBO_CAPACITY
{
    _ic_dbgprintf("IC Constructed   PARAMETER 3   %p\n", this);

    for(uint64_t i=0; i<p_size; i++)
    {
        m_memory[i] = p_array[i];
    }

    m_bits.sign = p_negative_value;
    m_size = p_size;
}

InfiniterCore::InfiniterCore(const InfiniterCore &p_source)
    : InfiniterMemory(p_source)
{
    _ic_dbgprintf("IC Constructed   COPY          %p\n", this);

    /// entire memory was copied in IM

    m_bits.sign = p_source.m_bits.sign;
    m_size = p_source.m_size;
}

InfiniterCore::InfiniterCore(InfiniterCore &&p_source) noexcept
    : InfiniterMemory(std::move(p_source))
{
    _ic_dbgprintf("IC Constructed   MOVE          %p\n", this);

    /// entire memory was moved in IM

    m_bits.sign = p_source.m_bits.sign;
    m_size = p_source.m_size;
}

InfiniterCore::~InfiniterCore() noexcept
{
    _ic_dbgprintf("IC DEL                         %p\n", this);

    /// everything was done in InfiniterMemory
}

void InfiniterCore::reset() noexcept
{
    InfiniterMemory::reset(); /// makes m_capacity == SBO_CAPACITY
    m_bits.sign = false;
    m_size = 1ull;

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

void InfiniterCore::optimize()
{
    this->shrink();
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

uint64_t InfiniterCore::setSize(uint64_t p_new_size) noexcept
{
    m_size = p_new_size > m_capacity ? m_capacity : p_new_size;

    return m_size;
}

void InfiniterCore::setSizeWithExtend(uint64_t p_new_size)
{
    if(p_new_size > m_capacity)
    {
        this->reserve( REALLOC_PADDING_SIZE(p_new_size) );
    }

    m_size = p_new_size;
}

void InfiniterCore::setSign(bool p_new_sign) noexcept
{
    m_bits.sign = p_new_sign;
}

void InfiniterCore::setPositiveSign() noexcept
{
    m_bits.sign = 0;
}

void InfiniterCore::setNegativeSign() noexcept
{
    m_bits.sign = 1;
}

void InfiniterCore::assign(uint64_t p_value, bool p_negative_value) noexcept
{
    m_memory[0] = p_value;

    m_bits.sign = p_negative_value;
    m_size = 1ull;
}

void InfiniterCore::assign(const cell_t *p_array, uint64_t p_size, bool p_negative_value)
{
    /// reserve more if needed
    /// UNLIKELY to speed up operations where memory isn't realocated
    if(UNLIKELY(p_size > m_capacity))
    {
        this->reserve( REALLOC_PADDING_SIZE(p_size) );
    }

    for(uint64_t i=0; i<p_size; i++)
    {
        m_memory[i] = p_array[i];
    }

    m_size = p_size;
    m_bits.sign = p_negative_value;
}

#if IC_ENABLE_DB_PRINT_METHOD
void InfiniterCore::dbg_print_data() const
{
    printf("IC obj: %p, capacity: %llu, size: %llu, sbo: %d, sign: %d\n",
           this, m_capacity, m_size,
           m_bits.sbo_active, m_bits.sign);
    for(uint64_t i=0; i<m_size; i++)
    {
        printf("%016llx ", m_memory[m_size-1-i]);
    }
    printf("\n");
}

void InfiniterCore::dbg_print_memory() const
{
    printf("IC obj: %p, capacity: %llu, size: %llu, sbo: %d, sign: %d\n",
           this, m_capacity, m_size,
           m_bits.sbo_active, m_bits.sign);
    for(uint64_t i=0; i<m_capacity; i++)
    {
        printf("%016llx ", m_memory[m_capacity-1-i]);
    }
    printf("\n");
}
#endif // IC_ENABLE_DB_PRINT_METHOD

InfiniterCore &InfiniterCore::operator =(const InfiniterCore &p_source)
{
    _ic_dbgprintf("IC Assigned      COPY          %p\n", this);

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
    _ic_dbgprintf("IC Assigned      MOVE          %p\n", this);

    if( &p_source != this )
    {
        InfiniterMemory::operator=(std::move(p_source));

        /// entire memory was moved in IM

        m_bits.sign = p_source.m_bits.sign;
        m_size = p_source.m_size;
    }

    return *this;

}


