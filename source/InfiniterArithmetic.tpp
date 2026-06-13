#pragma once

#include "InfiniterArithmetic.hpp"

#include "InfiniterShared.hpp"

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

template<typename InfiniterDerived>
InfiniterArithmetic<InfiniterDerived>::InfiniterArithmetic() noexcept
    : InfiniterBit<InfiniterDerived>()
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed   DEFAULT\n", this);
}

template<typename InfiniterDerived>
InfiniterArithmetic<InfiniterDerived>::InfiniterArithmetic(isize_t p_capacity)
    : InfiniterBit<InfiniterDerived>(p_capacity)
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed   PARAMETER isize_t\n", this);
}

template<typename InfiniterDerived>
InfiniterArithmetic<InfiniterDerived>::InfiniterArithmetic(isize_t p_capacity, icell_t p_value, bool p_negative_value)
    : InfiniterBit<InfiniterDerived>(p_capacity, p_value, p_negative_value)
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed   PARAMETER isize_t icell_t bool\n", this);
}

template<typename InfiniterDerived>
InfiniterArithmetic<InfiniterDerived>::InfiniterArithmetic(const icell_t *p_array, isize_t p_size, bool p_negative_value)
    : InfiniterBit<InfiniterDerived>(p_array, p_size, p_negative_value)
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed   PARAMETER icell_t* isize_t bool\n", this);
}

template<typename InfiniterDerived>
InfiniterArithmetic<InfiniterDerived>::InfiniterArithmetic(const InfiniterDerived &p_source)
    : InfiniterBit<InfiniterDerived>(p_source)
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed IA COPY\n", this);
}

template<typename InfiniterDerived>
InfiniterArithmetic<InfiniterDerived>::InfiniterArithmetic(InfiniterDerived &&p_source) noexcept
    : InfiniterBit<InfiniterDerived>(std::move(p_source))
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed IA MOVE\n", this);
}

template<typename InfiniterDerived>
InfiniterArithmetic<InfiniterDerived>::~InfiniterArithmetic() noexcept
{
    _ia_dbgprintf("--- DEBUG IA %p | Delete\n", this);

    /// everything was done in InfiniterMemory
}

template<typename InfiniterDerived>
void InfiniterArithmetic<InfiniterDerived>::incrementMagnitude()
{
    icell_t *data = this->getData();
    icell_t size = this->getSize();

    icell_t *cell_ptr = data;
    const icell_t *const last_cell = data + (size-1);

    /// find non fill cell
    while(*cell_ptr == ICELL_MAX && cell_ptr != last_cell)
    {
        (*cell_ptr)++; /// 11111111 -> (1)00000000
        cell_ptr++;
    }

    if(cell_ptr == last_cell && *cell_ptr == ICELL_MAX)
    {
        /// reach end and whole number look like this:
        /// 11111111 11111111 ... 11111111 11111111
        /// in the process was changed to this
        /// 00000000 00000000 ... 00000000 00000000
        /// add new cell

        (*cell_ptr)++; /// 11111111 -> (1)00000000

        size = this->setSizeWithExtend(size +1);
        data = this->getData(); /// update data - could change if memory was realocated
        cell_ptr = data + size -1;
    }

    /// cell_ptr will contain cell that could be modifed
    (*cell_ptr)++;
}

template<typename InfiniterDerived>
void InfiniterArithmetic<InfiniterDerived>::decrementMagnitude()
{
    icell_t *data = this->getData();
    icell_t size = this->getSize();

    icell_t *cell_ptr = data;
    const icell_t *const last_cell = data + (size-1);

    /// find non fill cell
    while(*cell_ptr == ICELL_C(0) && cell_ptr != last_cell)
    {
        (*cell_ptr)--; /// (1)00000000 -> 11111111
        cell_ptr++;
    }

    /// cell_ptr will contain cell that could be modifed
    (*cell_ptr)--;

    this->normalize();
}

template<typename InfiniterDerived>
void InfiniterArithmetic<InfiniterDerived>::addMagnitude(icell_t p_value)
{
    icell_t *data = this->getData();
    isize_t size = this->getSize();

    icell_t carry = p_value;
    for(isize_t i=0; i<size && carry; i++)
    {
        // __uint128_t sum = (__uint128_t)data[i] + carry;
        // data[i] = (icell_t)sum;
        // carry = (icell_t)(sum >> 64);
        carry = __builtin_add_overflow(data[i], carry, &data[i]);
    }

    if(carry)
    {
        size = this->setSizeWithExtend(size+1);
        this->getData()[size-1] = carry;
    }
}

template<typename InfiniterDerived>
void InfiniterArithmetic<InfiniterDerived>::subtractMagnitude(icell_t p_value)
{
    icell_t *data = this->getData();
    isize_t size = this->getSize();

    icell_t carry = p_value;
    for(isize_t i=0; i<size && carry; i++)
    {
        __uint128_t sum = (__uint128_t)data[i] + carry;
        data[i] = (icell_t)sum;
        carry = (icell_t)(sum >> 64);
    }

    if(carry)
    {
        size = this->setSizeWithExtend(size+1);
        this->getData()[size-1] = carry;
    }
}

template<typename InfiniterDerived>
void InfiniterArithmetic<InfiniterDerived>::addMagnitude(const InfiniterDerived &p_number)
{

}

template<typename InfiniterDerived>
void InfiniterArithmetic<InfiniterDerived>::subtractMagnitude(const InfiniterDerived &p_number)
{

}

template<typename InfiniterDerived>
void InfiniterArithmetic<InfiniterDerived>::increment()
{
    /// handle incrementing from negative
    if(this->getSign())
    {
        /// perform operation on absolute value
        this->decrementMagnitude();
        return;
    }

    this->incrementMagnitude();
}

template<typename InfiniterDerived>
void InfiniterArithmetic<InfiniterDerived>::decrement()
{
    /// handle decrementing from negative
    if(this->getSign())
    {
        /// perform operation on absolute value
        this->incrementMagnitude();
        return;
    }

    /// handle decrementing to negative
    /// as a second operation, because all negative decrementations
    /// will have one condition less
    if(this->is0())
    {
        this->increment();
        this->setNegativeSign();
        return;
    }

    this->decrementMagnitude();
}

template<typename InfiniterDerived>
void InfiniterArithmetic<InfiniterDerived>::add(icell_t p_value, bool p_negative_value)
{
    /// handle edge cases
    if(p_value == 0)
        return;
    if(p_value == 1)
    {
        if(p_negative_value)
            this->decrement();
        else
            this->increment();

        return;
    }

    /// this is negative
    if(this->getSign())
    {
        /// scalar is negative
        if(p_negative_value)
        {
            this->addMagnitude(p_value);
        }
        /// scalar is positive
        else
        {
            this->subtractMagnitude(p_value);
        }
    }
    /// this is positive
    else
    {
        /// scalar is negative
        if(p_negative_value)
        {
            this->subtractMagnitude(p_value);
        }
        /// scalar is positive
        else
        {
            this->addMagnitude(p_value);
        }
    }
}

template<typename InfiniterDerived>
void InfiniterArithmetic<InfiniterDerived>::subtract(icell_t p_value, bool p_negative_value)
{
    if(p_value == 0)
        return;
    if(p_value == 1)
    {
        if(p_negative_value)
            this->increment();
        else
            this->decrement();

        return;
    }

    /// this is negative
    if(this->getSign())
    {
        /// scalar is negative
        if(p_negative_value)
        {
            this->subtractMagnitude(p_value);
        }
        /// scalar is positive
        else
        {
            this->addMagnitude(p_value);
        }
    }
    /// this is positive
    else
    {
        /// scalar is negative
        if(p_negative_value)
        {
            this->addMagnitude(p_value);
        }
        /// scalar is positive
        else
        {
            this->subtractMagnitude(p_value);
        }
    }

}

template<typename InfiniterDerived>
void InfiniterArithmetic<InfiniterDerived>::add(const InfiniterDerived &p_number)
{
    // if(this->is0())
    // {
    //     this->assign(p_number);
    //     return;
    // }
    if(p_number.is0())
    {
        return;
    }
    if(p_number.is1())
    {
        if(p_number.getSign())
            this->decrement();
        else
            this->increment();
    }
}

template<typename InfiniterDerived>
void InfiniterArithmetic<InfiniterDerived>::subtract(const InfiniterDerived &p_number)
{

}

template<typename InfiniterDerived>
void InfiniterArithmetic<InfiniterDerived>::multiply(const InfiniterDerived &p_number)
{
    /// handle signs
    /// reverse sign only if other number is negative case
    if(p_number.getSign())
        this->negate();

    /// handle edge cases 0
    if(this->is0() || p_number.is0())
    {
        this->reset();
        return;
    }

    /// handle edge cases p_number
    if(p_number.isPositive1())
    {
        return;
    }
    if(p_number.isNegative1())
    {
        this->negate();
        return;
    }
    if(p_number.isPositive2())
    {
        this->pushLSB(IBIT_0);
        return;
    }
    if(p_number.isNegative2())
    {
        this->pushLSB(IBIT_0);
        this->negate();
        return;
    }

    /// handle edge cases this
    if(this->isPositive1())
    {
        this->assign(p_number);
        return;
    }
    if(this->isNegative1())
    {
        this->assign(p_number);
        this->negate();
        return;
    }
    if(this->isPositive2())
    {
        this->assign(p_number);
        this->pushLSB(IBIT_0);
        return;
    }
    if(this->isNegative2())
    {
        this->assign(p_number);
        this->pushLSB(IBIT_0);
        this->negate();
        return;
    }

}

template<typename InfiniterDerived>
void InfiniterArithmetic<InfiniterDerived>::divde(const InfiniterDerived &p_number)
{
    /// handle signs
    /// reverse sign only if other number is negative case
    if(p_number.getSign())
        this->negate();

    if(p_number.is0())
    {
        /// zero division exception
    }

    if(this->is0())
    {
        this->trim();
        return;
    }

    if(p_number.isPositive1())
    {
        this->trim();
        return;
    }
    if(p_number.isNegative1())
    {
        this->negate();
        return;
    }
    if(p_number.isPositive2())
    {
        this->shiftMSB(IBIT_0);
        return;
    }
    if(p_number.isNegative2())
    {
        this->shiftMSB(IBIT_0);
        this->negate();
        return;
    }

    /// comparison optimization
    // if()
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::operator ++()
{
    this->increment();
    return static_cast<InfiniterDerived&>(*this);
}

template<typename InfiniterDerived>
InfiniterDerived InfiniterArithmetic<InfiniterDerived>::operator ++(int)
{
    InfiniterDerived number(*this); /// static cast needed?
    this->increment();
    return std::move(number);
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::operator --()
{
    this->increment();
    return static_cast<InfiniterDerived&>(*this);
}

template<typename InfiniterDerived>
InfiniterDerived InfiniterArithmetic<InfiniterDerived>::operator --(int)
{
    InfiniterDerived number(*this); /// static cast needed?
    this->decrement();
    return std::move(number);
}


