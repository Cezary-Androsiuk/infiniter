#pragma once

#include "InfiniterCore.hpp"

#include "InfiniterShared.hpp"
#include "InfiniterException.hpp"

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


template<typename InfiniterDerived>
InfiniterCore<InfiniterDerived>::InfiniterCore() noexcept
    : InfiniterMemory()
{
    _ic_dbgprintf("--- DEBUG IC %p | Constructed   DEFAULT\n", this);

    m_data[0] = ICELL_C(0);
    m_bits.sign = false;
    m_size = ISIZE_C(1);
}

template<typename InfiniterDerived>
InfiniterCore<InfiniterDerived>::InfiniterCore(int64_t p_value)
    : InfiniterMemory() /// capacity has to be equal or greater than 1
{
    /// value only covers first cell, purpose of this is to initialize instance with 1 or other scalar values
    _ic_dbgprintf("--- DEBUG IC %p | Constructed   PARAMETER int64_t\n", this);

    /// Bit Twiddling Hack (Absolute Value)
    int64_t mask = p_value >> 63;
    icell_t value = (p_value ^ mask) - mask;

    /// assign value
    m_data[0] = value;

    m_bits.sign = mask;
    m_size = ISIZE_C(1);
}

template<typename InfiniterDerived>
InfiniterCore<InfiniterDerived>::InfiniterCore(int64_t p_value, isize_t p_capacity)
    : InfiniterMemory(p_capacity) /// capacity has to be equal or greater than 1
{
    /// value only covers first cell, purpose of this is to initialize instance with 1 or other scalar values
    _ic_dbgprintf("--- DEBUG IC %p | Constructed   PARAMETER int64_t isize_t\n", this);

    /// Bit Twiddling Hack (Absolute Value)
    int64_t mask = p_value >> 63;
    icell_t value = (p_value ^ mask) - mask;

    /// assign value
    m_data[0] = value;

    m_bits.sign = mask;
    m_size = ISIZE_C(1);
}

template<typename InfiniterDerived>
InfiniterCore<InfiniterDerived>::InfiniterCore(icell_t p_value, isize_t p_capacity, bool p_negative_value)
    : InfiniterMemory(p_capacity) /// ensures that final capacity will be grater or equal to SBO_CAPACITY
{
    /// value only covers first cell, purpose of this is to initialize instance with 1 or other scalar values
    ///
    _ic_dbgprintf("--- DEBUG IC %p | Constructed   PARAMETER icell_t isize_t bool\n", this);
    
    /// assign value
    m_data[0] = p_value;
    
    m_bits.sign = (m_data[0] == ICELL_C(0)) ? false : p_negative_value;
    m_size = ISIZE_C(1);
}

template<typename InfiniterDerived>
InfiniterCore<InfiniterDerived>::InfiniterCore(const icell_t *p_array, isize_t p_size, bool p_negative_value)
    : InfiniterMemory(p_size) /// ensures that final capacity will be grater or equal to SBO_CAPACITY
{
    _ic_dbgprintf("--- DEBUG IC %p | Constructed   PARAMETER icell_t* isize_t bool\n", this);

    icell_t is_zero = ICELL_C(0);

    for(isize_t i=0; i<p_size; i++)
    {
        m_data[i] = p_array[i];
        is_zero |= m_data[i];
    }

    m_bits.sign = is_zero ? false : p_negative_value;
    m_size = p_size;
    this->normalize();
}

template<typename InfiniterDerived>
InfiniterCore<InfiniterDerived>::InfiniterCore(const InfiniterDerived &p_source)
    : InfiniterMemory(p_source)
{
    _ic_dbgprintf("--- DEBUG IC %p | Constructed   COPY\n", this);

    /// entire memory was copied in IM

    m_bits.sign = p_source.m_bits.sign;
    m_size = p_source.m_size;
    this->normalize();
}

template<typename InfiniterDerived>
InfiniterCore<InfiniterDerived>::InfiniterCore(InfiniterDerived &&p_source) noexcept
    : InfiniterMemory(std::move(p_source))
{
    _ic_dbgprintf("--- DEBUG IC %p | Constructed   MOVE\n", this);

    /// entire memory was moved in IM

    m_bits.sign = p_source.m_bits.sign;
    m_size = p_source.m_size;
    this->normalize();
}

template<typename InfiniterDerived>
InfiniterCore<InfiniterDerived>::~InfiniterCore() noexcept
{
    _ic_dbgprintf("--- DEBUG IC %p | Delete\n", this);

    /// everything was done in InfiniterMemory
}

template<typename InfiniterDerived>
void InfiniterCore<InfiniterDerived>::reset() noexcept
{
    InfiniterMemory::reset(); /// makes m_capacity == SBO_CAPACITY
    m_bits.sign = false;
    m_size = ISIZE_C(1);
}

template<typename InfiniterDerived>
inline InfiniterDerived &InfiniterCore<InfiniterDerived>::getRef() noexcept
{
    return static_cast<InfiniterDerived &>(*this);
}

template<typename InfiniterDerived>
inline const InfiniterDerived &InfiniterCore<InfiniterDerived>::getCRef() const noexcept
{
    return static_cast<const InfiniterDerived &>(*this);
}

template<typename InfiniterDerived>
inline InfiniterDerived InfiniterCore<InfiniterDerived>::getCopy() const
{
    return InfiniterDerived(static_cast<const InfiniterDerived &>(*this));
}

template<typename InfiniterDerived>
void InfiniterCore<InfiniterDerived>::clear() noexcept
{
    m_bits.sign = false;
    for(isize_t i=0; i<m_size; i++)
    {
        m_data[i] = ICELL_C(0);
    }
}

template<typename InfiniterDerived>
void InfiniterCore<InfiniterDerived>::clearReserved() noexcept
{
    m_bits.sign = false;
    for(isize_t i=0; i<m_capacity; i++)
    {
        m_data[i] = ICELL_C(0);
    }
}

template<typename InfiniterDerived>
void InfiniterCore<InfiniterDerived>::reserve(isize_t p_new_capacity)
{
    InfiniterMemory::reserve(p_new_capacity);
}

template<typename InfiniterDerived>
void InfiniterCore<InfiniterDerived>::reserve(const InfiniterDerived &p_source)
{
    InfiniterMemory::reserve(p_source);
}

template<typename InfiniterDerived>
void InfiniterCore<InfiniterDerived>::extend(isize_t p_additional_capacity)
{
    InfiniterMemory::extend(p_additional_capacity);
}

template<typename InfiniterDerived>
void InfiniterCore<InfiniterDerived>::shrink()
{
    /// shrink to m_size
    InfiniterMemory::shrink(m_size);
}

template<typename InfiniterDerived>
void InfiniterCore<InfiniterDerived>::optimize()
{
    this->shrink();
}

template<typename InfiniterDerived>
void InfiniterCore<InfiniterDerived>::trim() noexcept
{
    /// iterate from back
    for(isize_t i=0; i<m_size; i++)
    {
        const isize_t i_rev = m_size - 1 - i;

        /// on first non 0 cell stop and reduce m_size by i
        if(m_data[i_rev] != ICELL_C(0))
        {
            if(i)
            {
                this->setSize(m_size-i);
            }
            return;
        }
    }
}

template<typename InfiniterDerived>
void InfiniterCore<InfiniterDerived>::normalize() noexcept
{
    this->trim();
    if(this->is0())
        this->setPositiveSign();

}

template<typename InfiniterDerived>
icell_t *InfiniterCore<InfiniterDerived>::getData() noexcept
{
    return m_data;
}

template<typename InfiniterDerived>
const icell_t *InfiniterCore<InfiniterDerived>::getData() const noexcept
{
    return m_data;
}

template<typename InfiniterDerived>
isize_t InfiniterCore<InfiniterDerived>::getSize() const noexcept
{
    return m_size;
}

template<typename InfiniterDerived>
isize_t InfiniterCore<InfiniterDerived>::getCapacity() const noexcept
{
    return m_capacity;
}

template<typename InfiniterDerived>
ibit_t InfiniterCore<InfiniterDerived>::getSign() const noexcept
{
    return m_bits.sign;
}

template<typename InfiniterDerived>
isize_t InfiniterCore<InfiniterDerived>::getRealSize() const noexcept
{
    isize_t real_size = m_size;
    while (real_size > 1 && m_data[real_size - 1] == ICELL_C(0))
    {
        real_size--;
    }
    return real_size;
}

template<typename InfiniterDerived>
isize_t InfiniterCore<InfiniterDerived>::setSize(isize_t p_new_size) noexcept
{
    isize_t new_size = std::min(p_new_size, m_capacity);

    /// clear extended memory to keep the same value (it could contain old junk after trimming size)
    std::fill_n(m_data + m_size, new_size-m_size, ICELL_C(0));

    return m_size = new_size;
}

template<typename InfiniterDerived>
isize_t InfiniterCore<InfiniterDerived>::setSizeWithExtend(isize_t p_new_size)
{
    if(p_new_size > m_capacity)
    {
        this->reserve( REALLOC_PADDING_SIZE(p_new_size) );
    }
    
    return this->setSize(p_new_size);
}

template<typename InfiniterDerived>
void InfiniterCore<InfiniterDerived>::setSign(bool p_new_sign) noexcept
{
    m_bits.sign = p_new_sign;
}

template<typename InfiniterDerived>
void InfiniterCore<InfiniterDerived>::setPositiveSign() noexcept
{
    m_bits.sign = 0;
}

template<typename InfiniterDerived>
void InfiniterCore<InfiniterDerived>::setNegativeSign() noexcept
{
    m_bits.sign = 1;
}

template<typename InfiniterDerived>
void InfiniterCore<InfiniterDerived>::negate() noexcept
{
    m_bits.sign = !m_bits.sign;
}

template<typename InfiniterDerived>
InfiniterDerived InfiniterCore<InfiniterDerived>::absoluteValue() const
{
    InfiniterDerived i(*this);
    i.setPositiveSign();
    return i; /// copy elision aka RVO (Return Value Optimization)
    /// using move is "pessimising move" and after C++17 copy elision is guaranteed
}

template<typename InfiniterDerived>
void InfiniterCore<InfiniterDerived>::absoluteValueAssign()
{
    this->setPositiveSign();
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterCore<InfiniterDerived>::assign(icell_t p_value, bool p_negative_value) noexcept
{
    m_data[0] = p_value;

    m_bits.sign = (m_data[0] == ICELL_C(0)) ? false : p_negative_value;
    m_size = ISIZE_C(1);

    return this->getRef();
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterCore<InfiniterDerived>::assign(const icell_t *p_array, isize_t p_size, bool p_negative_value)
{
    /// reserve more if needed
    /// UNLIKELY to speed up operations where memory isn't realocated
    if(UNLIKELY(p_size > m_capacity))
    {
        this->reserve( REALLOC_PADDING_SIZE(p_size) );
    }

    for(isize_t i=0; i<p_size; i++)
    {
        m_data[i] = p_array[i];
    }

    m_size = p_size;
    m_bits.sign = p_negative_value;

    this->normalize();

    return this->getRef();
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterCore<InfiniterDerived>::assign(const InfiniterDerived &p_source)
{
    if( &p_source != this )
    {
        InfiniterMemory::assign(p_source);

        /// entire memory was copied in IM

        m_bits.sign = p_source.m_bits.sign;
        m_size = p_source.m_size;

        this->normalize();
    }

    return this->getRef();
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterCore<InfiniterDerived>::assign(InfiniterDerived &&p_source)
{
    if( &p_source != this )
    {
        InfiniterMemory::assign(std::move(p_source));

        /// entire memory was moved in IM

        m_bits.sign = p_source.m_bits.sign;
        m_size = p_source.m_size;

        this->normalize();
    }

    return this->getRef();
}

template<typename InfiniterDerived>
inline bool InfiniterCore<InfiniterDerived>::equalMagnitude(icell_t p_scalar, int p_sign) const noexcept
{

}

template<typename InfiniterDerived>
inline bool InfiniterCore<InfiniterDerived>::differsMagnitude(icell_t p_scalar, int p_sign) const noexcept
{

}

template<typename InfiniterDerived>
inline bool InfiniterCore<InfiniterDerived>::greaterMagnitude(icell_t p_scalar, int p_sign) const noexcept
{

}

template<typename InfiniterDerived>
inline bool InfiniterCore<InfiniterDerived>::smallerMagnitude(icell_t p_scalar, int p_sign) const noexcept
{

}

template<typename InfiniterDerived>
inline bool InfiniterCore<InfiniterDerived>::greaterEqualMagnitude(icell_t p_scalar, int p_sign) const noexcept
{

}

template<typename InfiniterDerived>
inline bool InfiniterCore<InfiniterDerived>::smallerEqualMagnitude(icell_t p_scalar, int p_sign) const noexcept
{

}


template<typename InfiniterDerived>
inline bool InfiniterCore<InfiniterDerived>::equalMagnitude(const InfiniterDerived &p_right) const noexcept
{

}

template<typename InfiniterDerived>
inline bool InfiniterCore<InfiniterDerived>::differsMagnitude(const InfiniterDerived &p_right) const noexcept
{

}

template<typename InfiniterDerived>
inline bool InfiniterCore<InfiniterDerived>::greaterMagnitude(const InfiniterDerived &p_right) const noexcept
{

}

template<typename InfiniterDerived>
inline bool InfiniterCore<InfiniterDerived>::smallerMagnitude(const InfiniterDerived &p_right) const noexcept
{

}

template<typename InfiniterDerived>
inline bool InfiniterCore<InfiniterDerived>::greaterEqualMagnitude(const InfiniterDerived &p_right) const noexcept
{

}

template<typename InfiniterDerived>
inline bool InfiniterCore<InfiniterDerived>::smallerEqualMagnitude(const InfiniterDerived &p_right) const noexcept
{

}


template<typename InfiniterDerived>
inline bool InfiniterCore<InfiniterDerived>::equal(icell_t p_scalar, int p_sign) const noexcept
{
    // if(p_sign)
    // {
    //     /// user wants positive value and sign tells the number is negative
    //     if(p_sign > 0 && this->getSign()) return false;

    //     /// user wants negative value and sign tells the number is positive
    //     if(p_sign < 0 && !this->getSign()) return false;
    // }

    /// shorter and branchless condition
    if (p_sign != 0 && ((p_sign < 0) != m_bits.sign))
        return false;

    /// check LSB - most common case
    if(m_data[0] != p_scalar)
        return false;

    const isize_t size = m_size - 1; /// -1 excluded from loop
    /// iterate from MSB to LSB+1
    for(isize_t i=0; i<size; i++)
    {
        const isize_t i_rev = size - i;
        if(m_data[i_rev] != ICELL_C(0))
            return false;
    }
    return true;
}

template<typename InfiniterDerived>
inline bool InfiniterCore<InfiniterDerived>::differs(icell_t p_scalar, int p_sign) const noexcept
{

}

template<typename InfiniterDerived>
inline bool InfiniterCore<InfiniterDerived>::greater(icell_t p_scalar, int p_sign) const noexcept
{

}

template<typename InfiniterDerived>
inline bool InfiniterCore<InfiniterDerived>::smaller(icell_t p_scalar, int p_sign) const noexcept
{

}

template<typename InfiniterDerived>
inline bool InfiniterCore<InfiniterDerived>::greaterEqual(icell_t p_scalar, int p_sign) const noexcept
{

}

template<typename InfiniterDerived>
inline bool InfiniterCore<InfiniterDerived>::smallerEqual(icell_t p_scalar, int p_sign) const noexcept
{

}

template<typename InfiniterDerived>
bool InfiniterCore<InfiniterDerived>::equal(const InfiniterDerived &p_right) const noexcept
{
    if(this == &p_right)
        return true;

    /// handle signs
    if(m_bits.sign != p_right.m_bits.sign)
        return false;

    /// handle different sizes
    isize_t left_size = this->getRealSize();
    isize_t right_size = p_right.getRealSize();

    if(left_size != right_size)
        return false;

    /// handle data
    const icell_t *right_data = p_right.m_data;
    for(isize_t i=0; i<left_size; i++)
    {
        if(m_data[i] != right_data[i])
            return false;
    }

    return true;
}

template<typename InfiniterDerived>
bool InfiniterCore<InfiniterDerived>::differs(const InfiniterDerived &p_right) const noexcept
{
    if(this == &p_right)
        return false;

    /// handle signs
    if(m_bits.sign != p_right.m_bits.sign)
        return true;

    /// handle different sizes
    isize_t left_size = this->getRealSize();
    isize_t right_size = p_right.getRealSize();

    if(left_size != right_size)
        return true;

    /// handle data
    const icell_t *right_data = p_right.m_data;
    for(isize_t i=0; i<left_size; i++)
    {
        if(m_data[i] != right_data[i])
            return true;
    }

    return false;
}

template<typename InfiniterDerived>
bool InfiniterCore<InfiniterDerived>::greater(const InfiniterDerived &p_right) const noexcept
{
    if(this == &p_right)
        return false;

    /// handle signs
    if(m_bits.sign != p_right.m_bits.sign)
    {
        return m_bits.sign;
    }

    /// handle different sizes
    isize_t left_size = this->getRealSize();
    isize_t right_size = p_right.getRealSize();

    if(left_size > right_size)
        return true;

    /// handle data - iterate from MSB
    const icell_t *right_data = p_right.m_data;
    for(isize_t i=0; i<left_size; i++)
    {
        isize_t i_rev = left_size - 1 - i;
        if(m_data[i_rev] != right_data[i_rev])
        {
            return m_data[i_rev] > right_data[i_rev];
        }
    }

    return false;
}

template<typename InfiniterDerived>
bool InfiniterCore<InfiniterDerived>::smaller(const InfiniterDerived &p_right) const noexcept
{
    if(this == &p_right)
        return false;

    /// handle signs
    if(m_bits.sign != p_right.m_bits.sign)
    {
        return !m_bits.sign;
    }

    /// handle different sizes
    isize_t left_size = this->getRealSize();
    isize_t right_size = p_right.getRealSize();

    if(left_size < right_size)
        return true;

    /// handle data - iterate from MSB
    const icell_t *right_data = p_right.m_data;
    for(isize_t i=0; i<left_size; i++)
    {
        isize_t i_rev = left_size - 1 - i;
        if(m_data[i_rev] != right_data[i_rev])
        {
            return m_data[i_rev] < right_data[i_rev];
        }
    }

    return false;
}

template<typename InfiniterDerived>
bool InfiniterCore<InfiniterDerived>::greaterEqual(const InfiniterDerived &p_right) const noexcept
{
    if(this == &p_right)
        return true;

    /// handle signs
    if(m_bits.sign != p_right.m_bits.sign)
    {
        return m_bits.sign;
    }

    /// handle different sizes
    isize_t left_size = this->getRealSize();
    isize_t right_size = p_right.getRealSize();

    if(left_size > right_size)
        return true;

    /// handle data - iterate from MSB
    const icell_t *right_data = p_right.m_data;
    for(isize_t i=0; i<left_size; i++)
    {
        isize_t i_rev = left_size - 1 - i;
        if(m_data[i_rev] != right_data[i_rev])
        {
            return m_data[i_rev] > right_data[i_rev];
        }
    }

    return true;
}

template<typename InfiniterDerived>
bool InfiniterCore<InfiniterDerived>::smallerEqual(const InfiniterDerived &p_right) const noexcept
{
    if(this == &p_right)
        return true;

    /// handle signs
    if(m_bits.sign != p_right.m_bits.sign)
    {
        return !m_bits.sign;
    }

    /// handle different sizes
    isize_t left_size = this->getRealSize();
    isize_t right_size = p_right.getRealSize();

    if(left_size < right_size)
        return true;

    /// handle data - iterate from MSB
    const icell_t *right_data = p_right.m_data;
    for(isize_t i=0; i<left_size; i++)
    {
        isize_t i_rev = left_size - 1 - i;
        if(m_data[i_rev] != right_data[i_rev])
        {
            return m_data[i_rev] < right_data[i_rev];
        }
    }

    return true;
}

template<typename InfiniterDerived>
bool InfiniterCore<InfiniterDerived>::is0() const noexcept
{
    /// check LSB - most common case
    if(m_data[0] != ICELL_C(0))
        return false;

    const isize_t size = m_size - 1; /// -1 excluded from loop
    /// iterate from MSB to LSB+1
    for(isize_t i=0; i<size; i++)
    {
        const isize_t i_rev = size - i;
        if(m_data[i_rev] != ICELL_C(0))
            return false;
    }
    return true;
}

template<typename InfiniterDerived>
bool InfiniterCore<InfiniterDerived>::is1() const noexcept
{
    return this->equal(1, 0);
}

template<typename InfiniterDerived>
bool InfiniterCore<InfiniterDerived>::is2() const noexcept
{
    return this->equal(2, 0);
}

template<typename InfiniterDerived>
bool InfiniterCore<InfiniterDerived>::isPositive1() const noexcept
{
    return this->equal(1, 1);
}

template<typename InfiniterDerived>
bool InfiniterCore<InfiniterDerived>::isNegative1() const noexcept
{
    return this->equal(1, -1);
}

template<typename InfiniterDerived>
bool InfiniterCore<InfiniterDerived>::isPositive2() const noexcept
{
    return this->equal(2, 1);
}

template<typename InfiniterDerived>
bool InfiniterCore<InfiniterDerived>::isNegative2() const noexcept
{
    return this->equal(2, -1);
}

template<typename InfiniterDerived>
bool InfiniterCore<InfiniterDerived>::toBool() const noexcept
{
    /// check LSB - most common case
    if(m_data[0] != ICELL_C(0))
        return false;

    const isize_t size = m_size - 1; /// -1 excluded from loop
    /// iterate from MSB to LSB+1
    for(isize_t i=0; i<size; i++)
    {
        const isize_t i_rev = size - i;
        if(m_data[i_rev] != ICELL_C(0))
            return true;
    }
    return false;
}

template<typename InfiniterDerived>
icell_t &InfiniterCore<InfiniterDerived>::getCell(isize_t p_cell_index)
{
    if(p_cell_index >= m_size)
    {
        throw InfiniterException::OutOfRange(p_cell_index, 0, m_size-1);
    }

    return this->getData()[p_cell_index];
}

template<typename InfiniterDerived>
const icell_t &InfiniterCore<InfiniterDerived>::getCell(isize_t p_cell_index) const
{
    if(p_cell_index >= m_size)
    {
        throw InfiniterException::OutOfRange(p_cell_index, 0, m_size-1);
    }

    return this->getData()[p_cell_index];
}

template<typename InfiniterDerived>
icell_t &InfiniterCore<InfiniterDerived>::getCellUnsafe(isize_t p_cell_index) noexcept
{
    return this->getData()[p_cell_index];
}

template<typename InfiniterDerived>
const icell_t &InfiniterCore<InfiniterDerived>::getCellUnsafe(isize_t p_cell_index) const noexcept
{
    return this->getData()[p_cell_index];
}

#if IC_ENABLE_DB_PRINT_METHOD
template<typename InfiniterDerived>
void InfiniterCore<InfiniterDerived>::dbg_print_data() const
{
    printf("--- DEBUG IC obj: %p, capacity: %llu, size: %llu, sbo: %d, sign: %d\n",
           this, m_capacity, m_size,
           m_bits.sbo_active, m_bits.sign);
    for(isize_t i=0; i<m_size; i++)
    {
        printf("%016llx ", m_data[m_size - ISIZE_C(1) - i]);
    }
    printf("\n");
}

template<typename InfiniterDerived>
void InfiniterCore<InfiniterDerived>::dbg_print_memory() const
{
    printf("--- DEBUG IC obj: %p, capacity: %llu, size: %llu, sbo: %d, sign: %d\n",
           this, m_capacity, m_size,
           m_bits.sbo_active, m_bits.sign);
    for(isize_t i=0; i<m_capacity; i++)
    {
        printf("%016llx ", m_data[m_capacity - ISIZE_C(1) - i]);
    }
    printf("\n");
}
#endif // IC_ENABLE_DB_PRINT_METHOD

template<typename InfiniterDerived>
InfiniterDerived &InfiniterCore<InfiniterDerived>::operator =(const InfiniterDerived &p_source)
{
    _ic_dbgprintf("--- DEBUG IC %p | Assigned      COPY\n", this);

    if( &p_source != this )
    {
        InfiniterMemory::assign(p_source);

        /// entire memory was copied in IM

        m_bits.sign = p_source.m_bits.sign;
        m_size = p_source.m_size;
    }

    return static_cast<InfiniterDerived&>(*this);
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterCore<InfiniterDerived>::operator =(InfiniterDerived &&p_source)
{
    _ic_dbgprintf("--- DEBUG IC %p | Assigned      MOVE\n", this);

    if( &p_source != this )
    {
        InfiniterMemory::assign(std::move(p_source));

        /// entire memory was moved in IM

        m_bits.sign = p_source.m_bits.sign;
        m_size = p_source.m_size;
    }

    return static_cast<InfiniterDerived&>(*this);
}

template<typename InfiniterDerived>
InfiniterCore<InfiniterDerived>::operator bool() const noexcept
{
    return this->toBool();
}

template<typename InfiniterDerived>
inline InfiniterDerived InfiniterCore<InfiniterDerived>::operator ~() const noexcept
{
    InfiniterDerived number(*this);
    number.negate();
    return number; /// copy elision aka RVO (Return Value Optimization)
    /// using move is "pessimising move" and after C++17 copy elision is guaranteed
}

template<typename InfiniterDerived>
bool InfiniterCore<InfiniterDerived>::operator ==(const InfiniterDerived &p_source) const noexcept
{
    return this->equal(p_source);
}

template<typename InfiniterDerived>
bool InfiniterCore<InfiniterDerived>::operator !=(const InfiniterDerived &p_source) const noexcept
{
    return this->differs(p_source);
}

template<typename InfiniterDerived>
bool InfiniterCore<InfiniterDerived>::operator >(const InfiniterDerived &p_source) const noexcept
{
    return this->greater(p_source);
}

template<typename InfiniterDerived>
bool InfiniterCore<InfiniterDerived>::operator <(const InfiniterDerived &p_source) const noexcept
{
    return this->smaller(p_source);
}

template<typename InfiniterDerived>
bool InfiniterCore<InfiniterDerived>::operator >=(const InfiniterDerived &p_source) const noexcept
{
    return this->greaterEqual(p_source);
}

template<typename InfiniterDerived>
bool InfiniterCore<InfiniterDerived>::operator <=(const InfiniterDerived &p_source) const noexcept
{
    return this->smallerEqual(p_source);
}

template<typename InfiniterDerived>
icell_t &InfiniterCore<InfiniterDerived>::operator [](isize_t p_cell_index)
{
    return this->getCell(p_cell_index);
}

template<typename InfiniterDerived>
const icell_t &InfiniterCore<InfiniterDerived>::operator [](isize_t p_cell_index) const
{
    return this->getCell(p_cell_index);
}

