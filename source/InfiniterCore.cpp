#include "InfiniterCore.hpp"

#include "InfiniterShared.hpp"

#include <utility> // std::move

/// _ic_dbgprintf("--- DEBUG IC %p | Constructed   DEFAULT\n", this);
/// _ic_dbgprintf("--- DEBUG IC %p | Constructed   PARAMETER\n", this);
/// _ic_dbgprintf("--- DEBUG IC %p | Constructed   COPY\n", this);
/// _ic_dbgprintf("--- DEBUG IC %p | Constructed   MOVE\n", this);
/// _ic_dbgprintf("--- DEBUG IC %p | Assigned      COPY\n", this);
/// _ic_dbgprintf("--- DEBUG IC %p | Assigned      MOVE\n", this);
/// _ic_dbgprintf("--- DEBUG IC %p | Delete\n", this);
/// _ic_dbgprintf("--- DEBUG IC %p | Other         (...)\n", this);
#if IC_DEBUG_EXECUTION_PRINT
#include <cstdio>
#define _ic_dbgprintf(...) printf(__VA_ARGS__);
#else // IC_DEBUG_EXECUTION_PRINT
#define _ic_dbgprintf(...)
#endif // IC_DEBUG_EXECUTION_PRINT

InfiniterCore::InfiniterCore() noexcept
    : InfiniterMemory()
{
    _ic_dbgprintf("--- DEBUG IC %p | Constructed   DEFAULT\n", this);

    m_bits.sign = false;
    m_size = ISIZE_C(1);
}

InfiniterCore::InfiniterCore(isize_t p_capacity)
    : InfiniterMemory(p_capacity) /// ensures that final capacity will be grater or equal to SBO_CAPACITY
{
    _ic_dbgprintf("--- DEBUG IC %p | Constructed   PARAMETER 1\n", this);

    m_bits.sign = false;
    m_size = ISIZE_C(1);
}

InfiniterCore::InfiniterCore(isize_t p_capacity, icell_t p_value, bool p_negative_value)
    : InfiniterMemory(p_capacity) /// ensures that final capacity will be grater or equal to SBO_CAPACITY
{
    _ic_dbgprintf("--- DEBUG IC %p | Constructed   PARAMETER 2\n", this);

    m_bits.sign = p_negative_value;
    m_size = ISIZE_C(1);

    /// assign value
    m_memory[0] = p_value;
}

InfiniterCore::InfiniterCore(const icell_t *p_array, isize_t p_size, bool p_negative_value)
    : InfiniterMemory(p_size) /// ensures that final capacity will be grater or equal to SBO_CAPACITY
{
    _ic_dbgprintf("--- DEBUG IC %p | Constructed   PARAMETER 3\n", this);

    for(isize_t i=0; i<p_size; i++)
    {
        m_memory[i] = p_array[i];
    }

    m_bits.sign = p_negative_value;
    m_size = p_size;
}

InfiniterCore::InfiniterCore(const InfiniterCore &p_source)
    : InfiniterMemory(p_source)
{
    _ic_dbgprintf("--- DEBUG IC %p | Constructed   COPY\n", this);

    /// entire memory was copied in IM

    m_bits.sign = p_source.m_bits.sign;
    m_size = p_source.m_size;
}

InfiniterCore::InfiniterCore(InfiniterCore &&p_source) noexcept
    : InfiniterMemory(std::move(p_source))
{
    _ic_dbgprintf("--- DEBUG IC %p | Constructed   MOVE\n", this);

    /// entire memory was moved in IM

    m_bits.sign = p_source.m_bits.sign;
    m_size = p_source.m_size;
}

InfiniterCore::~InfiniterCore() noexcept
{
    _ic_dbgprintf("--- DEBUG IC %p | Delete\n", this);

    /// everything was done in InfiniterMemory
}

void InfiniterCore::reset() noexcept
{
    InfiniterMemory::reset(); /// makes m_capacity == SBO_CAPACITY
    m_bits.sign = false;
    m_size = ISIZE_C(1);

    for(isize_t i=0; i<m_capacity; i++)
    {
        m_memory[i] = ICELL_C(1);
    }
}

void InfiniterCore::clear() noexcept
{
    for(isize_t i=0; i<m_size; i++)
    {
        m_memory[i] = ICELL_C(0);
    }
}

void InfiniterCore::clearReserved() noexcept
{
    for(isize_t i=0; i<m_capacity; i++)
    {
        m_memory[i] = ICELL_C(0);
    }
}

void InfiniterCore::reserve(isize_t p_new_capacity)
{
    InfiniterMemory::reserve(p_new_capacity);
}

void InfiniterCore::reserve(const InfiniterCore &p_source)
{
    InfiniterMemory::reserve(p_source);
}

void InfiniterCore::extend(isize_t p_additional_capacity)
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

void InfiniterCore::trim()
{
    /// iterate from back
    for(isize_t i=0; i<m_size; i++)
    {
        const isize_t i_rev = m_size - ISIZE_C(1) - i;

        /// on first non 0 cell stop and reduce m_size by i
        if(m_memory[i_rev] != ICELL_C(0))
        {
            if(i)
            {
                this->setSize(m_size-i);
            }
            return;
        }
    }
}

icell_t *InfiniterCore::getData() noexcept
{
    return m_memory;
}

const icell_t *InfiniterCore::getData() const noexcept
{
    return m_memory;
}

isize_t InfiniterCore::getSize() const noexcept
{
    return m_size;
}

isize_t InfiniterCore::getCapacity() const noexcept
{
    return m_capacity;
}

ibit_t InfiniterCore::getSign() const noexcept
{
    return m_bits.sign;
}

isize_t InfiniterCore::setSize(isize_t p_new_size) noexcept
{
    m_size = p_new_size > m_capacity ? m_capacity : p_new_size;

    return m_size;
}

isize_t InfiniterCore::setSizeWithExtend(isize_t p_new_size)
{
    if(p_new_size > m_capacity)
    {
        this->reserve( REALLOC_PADDING_SIZE(p_new_size) );
    }

    m_size = p_new_size;
    return m_size;
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

void InfiniterCore::assign(icell_t p_value, bool p_negative_value) noexcept
{
    m_memory[0] = p_value;

    m_bits.sign = p_negative_value;
    m_size = ISIZE_C(1);
}

void InfiniterCore::assign(const icell_t *p_array, isize_t p_size, bool p_negative_value)
{
    /// reserve more if needed
    /// UNLIKELY to speed up operations where memory isn't realocated
    if(UNLIKELY(p_size > m_capacity))
    {
        this->reserve( REALLOC_PADDING_SIZE(p_size) );
    }

    for(isize_t i=0; i<p_size; i++)
    {
        m_memory[i] = p_array[i];
    }

    m_size = p_size;
    m_bits.sign = p_negative_value;
}

#if IC_ENABLE_DB_PRINT_METHOD
void InfiniterCore::dbg_print_data() const
{
    printf("--- DEBUG IC obj: %p, capacity: %llu, size: %llu, sbo: %d, sign: %d\n",
           this, m_capacity, m_size,
           m_bits.sbo_active, m_bits.sign);
    for(isize_t i=0; i<m_size; i++)
    {
        printf("%016llx ", m_memory[m_size - ISIZE_C(1) - i]);
    }
    printf("\n");
}

void InfiniterCore::dbg_print_memory() const
{
    printf("--- DEBUG IC obj: %p, capacity: %llu, size: %llu, sbo: %d, sign: %d\n",
           this, m_capacity, m_size,
           m_bits.sbo_active, m_bits.sign);
    for(isize_t i=0; i<m_capacity; i++)
    {
        printf("%016llx ", m_memory[m_capacity - ISIZE_C(1) - i]);
    }
    printf("\n");
}
#endif // IC_ENABLE_DB_PRINT_METHOD

InfiniterCore &InfiniterCore::operator =(const InfiniterCore &p_source)
{
    _ic_dbgprintf("--- DEBUG IC %p | Assigned      COPY\n", this);

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
    _ic_dbgprintf("--- DEBUG IC %p | Assigned      MOVE\n", this);

    if( &p_source != this )
    {
        InfiniterMemory::operator=(std::move(p_source));

        /// entire memory was moved in IM

        m_bits.sign = p_source.m_bits.sign;
        m_size = p_source.m_size;
    }

    return *this;

}


