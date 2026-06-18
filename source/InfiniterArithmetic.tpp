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
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::incrementMagnitude()
{
    icell_t *data = this->getData();
    icell_t size = this->getRealSize();

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

    this->normalize();
    return *this;
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::decrementMagnitude()
{
    icell_t *data = this->getData();
    icell_t size = this->getRealSize();

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
    return *this;
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::addMagnitude(icell_t p_right)
{
    icell_t *data = this->getData();
    isize_t size = this->getRealSize();

    icell_t carry = p_right;
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

    this->normalize();
    return *this;
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::subtractMagnitude(icell_t p_right)
{
    icell_t *data = this->getData();
    isize_t size = this->getRealSize();

    icell_t carry = p_right;
    for(isize_t i=0; i<size && carry; i++)
    {
        __uint128_t sum = static_cast<__uint128_t>(data[i]) + carry;
        data[i] = static_cast<icell_t>(sum);
        carry = static_cast<icell_t>(sum >> icell_bits);
    }

    if(carry)
    {
        size = this->setSizeWithExtend(size+1);
        this->getData()[size-1] = carry;
    }

    this->normalize();
    return *this;
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::addMagnitude(const InfiniterDerived &p_right)
{
    /// block to approximate result size
    {
        isize_t lsize = this->getRealSize();
        isize_t rsize = p_right.getRealSize();

        isize_t larger_size = lsize >= rsize ? lsize : rsize;
        const icell_t *larger_data = lsize >= rsize ? this->getData() : p_right.getData();

        isize_t extra_cell = !!(larger_data[larger_size-1] & M100);  /// +1 if needed
        isize_t out_size = larger_size + extra_cell;

        this->setSizeWithExtend(out_size);
    }

    /// at this point left number has greater or equal size to right number

    icell_t *ldata = this->getData();
    const icell_t *rdata = p_right.getData();

    isize_t lsize = this->getSize();
    isize_t rsize = p_right.getSize();

    icell_t carry = ICELL_C(0);

    /// add common part
    for(isize_t i=0; i<rsize; i++)
    {
        __uint128_t sum = static_cast<__uint128_t>(ldata[i]) + rdata[i] + carry;

        ldata[i] = static_cast<icell_t>(sum);
        carry = static_cast<icell_t>(sum >> icell_bits);
    }

    /// move the rest (with carry)
    for(isize_t i=rsize; i<lsize && carry; i++)
    {
        __uint128_t sum = static_cast<__uint128_t>(ldata[i]) + carry;

        ldata[i] = static_cast<icell_t>(sum);
        carry = static_cast<icell_t>(sum >> icell_bits);
    }

    this->normalize();
    return *this;
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::subtractMagnitude(const InfiniterDerived &p_right)
{
    /// assert |this| >= |p_right|

    icell_t *ldata = this->getData();
    const icell_t *rdata = p_right.getData();

    isize_t lsize = this->getSize();
    isize_t rsize = p_right.getSize();

    icell_t borrow = ICELL_C(0);

    /// subtract common part
    for(isize_t i = 0; i < rsize; i++)
    {
        /// handling subtracting is easier with signed value
        __int128_t diff = static_cast<__int128_t>(ldata[i]) - rdata[i] - borrow;

        ldata[i] = static_cast<icell_t>(diff);
        borrow = static_cast<uint64_t>(static_cast<__uint128_t>(diff) >> 127);
    }

    /// propagate borrow (move the rest)
    for(isize_t i = rsize; i < lsize && borrow; i++)
    {
        __int128_t diff = static_cast<__int128_t>(ldata[i]) - borrow;

        ldata[i] = static_cast<icell_t>(diff);
        borrow = static_cast<uint64_t>(static_cast<__uint128_t>(diff) >> 127);
    }

    this->normalize();
    return *this;
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::increment()
{
    /// handle incrementing from negative
    if(this->getSign())
    {
        /// perform operation on absolute value
        return this->decrementMagnitude();  /// has normalize
    }

    return this->incrementMagnitude();  /// has normalize

}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::decrement()
{
    /// handle decrementing from negative
    if(this->getSign())
    {
        /// perform operation on absolute value
        return this->incrementMagnitude();  /// has normalize
    }

    /// handle decrementing to negative
    /// as a second operation, because all negative decrementations
    /// will have one condition less
    if(this->is0())
    {
        this->setNegativeSign();
        return this->incrementMagnitude(); /// has normalize
    }
    
    return this->decrementMagnitude();  /// has normalize
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::add(icell_t p_right, bool p_negative_value)
{
    /// handle edge cases
    if(p_right == 0)
        return *this;
    if(p_right == 1)
    {
        if(p_negative_value)
            return this->decrement();  /// has normalize
        else
            return this->increment();  /// has normalize
    }
    ///  1 +  1   ->    (1+1)
    /// -1 +  1   ->   -(1-1)
    ///  1 + -1   ->    (1-1)
    /// -1 + -1   ->   -(1+1)
    ///
    ///  2 +  1   ->    (2+1)
    /// -2 +  1   ->   -(2-1)
    ///  2 + -1   ->    (2-1)
    /// -2 + -1   ->   -(2+1)
    ///
    ///  1 +  2   ->    (1+2)   ->    (2+1)
    /// -1 +  2   ->   -(1-2)   ->    (2-1)
    ///  1 + -2   ->    (1-2)   ->   -(2-1)
    /// -1 + -2   ->   -(1+2)   ->   -(2+1)

    /// addition:
    /// this is negative
    if(this->getSign())
    {
        /// scalar is negative
        if(p_negative_value)
        {
            this->setNegativeSign();
            return this->addMagnitude(p_right);  /// has normalize
        }
        
        /// scalar is positive
        // -2 + 1    ->   1 - 2   ->   -(2 - 1)
        if(this->greaterMagnitude(p_right, p_negative_value))
        {
            InfiniterDerived value(p_right);
            value.subtractMagnitude(*this).negate(); /// has normalize
            value.setNegativeSign();
            return this->assign(std::move(value)); /// has normalize
        }
        else
        {
            this->setPositiveSign();
            return this->subtractMagnitude(p_right);  /// has normalize
        }
    }
    /// this is positive
    else
    {
        /// scalar is positive
        if(!p_negative_value)
        {
            this->setPositiveSign();
            return this->addMagnitude(p_right);  /// has normalize
        }
        
        /// scalar is negative
        // 1 + -2   ->   1 - 2   ->   -(2 - 1)
        if(this->smallerMagnitude(p_right, p_negative_value))
        {
            InfiniterDerived value(p_right);
            value.subtractMagnitude(*this).negate(); /// has normalize
            value.setNegativeSign();
            return this->assign(std::move(value)); /// has normalize
        }
        else
        {

            this->setPositiveSign();
            return this->subtractMagnitude(p_right);  /// has normalize
        }
    }


}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::subtract(icell_t p_right, bool p_negative_value)
{
    if(p_right == 0)
    {
        this->normalize();
        return;
    }
    if(p_right == 1)
    {
        if(p_negative_value)
            this->increment();  /// has normalize
        else
            this->decrement();  /// has normalize

        return;
    }
    ///  1 -  1   ->    (1-1)
    /// -1 -  1   ->   -(1+1)
    ///  1 - -1   ->    (1+1)
    /// -1 - -1   ->   -(1-1)
    ///
    ///  2 -  1   ->    (2-1)
    /// -2 -  1   ->   -(2+1)
    ///  2 - -1   ->    (2+1)
    /// -2 - -1   ->   -(2-1)
    ///
    ///  1 -  2   ->    (1-2)   ->   -(2-1)
    /// -1 -  2   ->   -(1+2)   ->   -(2+1)
    ///  1 - -2   ->    (1+2)   ->    (2+1)
    /// -1 - -2   ->   -(1-2)   ->    (2-1)

    /// subtraction:
    /// this is negative
    if(this->getSign())
    {
        /// scalar is negative
        if(p_negative_value)
        {
            this->subtractMagnitude(p_right);  /// has normalize
        }
        /// scalar is positive
        else
        {
            this->addMagnitude(p_right);  /// has normalize
        }
    }
    /// this is positive
    else
    {
        /// scalar is negative
        if(p_negative_value)
        {
            this->addMagnitude(p_right);  /// has normalize
        }
        /// scalar is positive
        else
        {
            this->subtractMagnitude(p_right);  /// has normalize
        }
    }

}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::add(const InfiniterDerived &p_right)
{
    if(this->is0())
    {
        this->assign(p_right);
        this->normalize();
        return;
    }
    if(this->is1())
    {
        this->assign(p_right);
        if(p_right.getSign())
            this->decrement();  /// has normalize
        else
            this->increment();  /// has normalize

        return;
    }
    if(p_right.getRealSize() == 1)
    {
        if(p_right.is0())
        {
            this->normalize();
            return;
        }
        if(p_right.is1())
        {
            if(p_right.getSign())
                this->decrement();  /// has normalize
            else
                this->increment();  /// has normalize

            return;
        }
        if(p_right.getSign())
            this->subtract(p_right.getData()[0]);  /// has normalize
        else
            this->add(p_right.getData()[0]);  /// has normalize

        return;
    }

    /// addition:
    /// this is negative
    if(this->getSign())
    {
        /// scalar is negative
        if(p_right.getSign())
        {
            this->addMagnitude(p_right);  /// has normalize
        }
        /// scalar is positive
        else
        {
            this->subtractMagnitude(p_right);  /// has normalize
        }
    }
    /// this is positive
    else
    {
        /// scalar is negative
        if(p_right.getSign())
        {
            this->subtractMagnitude(p_right);  /// has normalize
        }
        /// scalar is positive
        else
        {
            this->addMagnitude(p_right);  /// has normalize
        }
    }
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::subtract(const InfiniterDerived &p_right)
{
    if(this->is0())
    {
        this->assign(p_right);
        this->normalize();
        return;
    }
    if(this->is1())
    {
        this->assign(p_right);
        if(p_right.getSign())
            this->increment();  /// has normalize
        else
            this->decrement();  /// has normalize

        return;
    }
    if(p_right.getRealSize() == 1)
    {
        if(p_right.is0())
        {
            this->normalize();
            return;
        }
        if(p_right.is1())
        {
            if(p_right.getSign())
                this->increment();  /// has normalize
            else
                this->decrement();  /// has normalize

            return;
        }
        if(p_right.getSign())
            this->subtract(p_right.getData()[0]);  /// has normalize
        else
            this->add(p_right.getData()[0]);  /// has normalize

        return;
    }

    /// subtraction:
    /// this is negative
    if(this->getSign())
    {
        /// scalar is negative
        if(p_right.getSign())
        {
            this->subtractMagnitude(p_right);  /// has normalize
        }
        /// scalar is positive
        else
        {
            this->addMagnitude(p_right);  /// has normalize
        }
    }
    /// this is positive
    else
    {
        /// scalar is negative
        if(p_right.getSign())
        {
            this->addMagnitude(p_right);  /// has normalize
        }
        /// scalar is positive
        else
        {
            this->subtractMagnitude(p_right);  /// has normalize
        }
    }
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::multiply(const InfiniterDerived &p_right)
{
    /// handle signs
    /// reverse sign only if other number is negative case
    if(p_right.getSign())
        this->negate();

    if(p_right.getRealSize() == 1)
    {
        /// handle edge cases p_right
        if(p_right.is0())
        {
            this->reset();
            return;
        }
        if(p_right.is1())
        {
            this->normalize();
            return;
        }
        if(p_right.is2())
        {
            this->pushLSB(IBIT_0);
            this->normalize();
            return;
        }
    }
    if(this->getRealSize() == 1)
    {
        /// handle edge cases this
        if(this->is0())
        {
            this->reset();
            return;
        }
        if(this->is1())
        {
            this->assign(p_right);
            this->normalize();
            return;
        }
        if(this->is2())
        {
            this->assign(p_right);
            this->pushLSB(IBIT_0);
            this->normalize();
            return;
        }
    }


    /// multiplication:


}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::divde(const InfiniterDerived &p_right)
{
    /// handle signs
    /// reverse sign only if other number is negative case
    if(p_right.getSign())
        this->negate();

    if(p_right.is0())
    {
        /// zero division exception
    }

    if(this->is0())
    {
        this->reset();
        return;
    }

    if(p_right.isPositive1())
    {
        this->trim();
        return;
    }
    if(p_right.isNegative1())
    {
        this->negate();
        return;
    }
    if(p_right.isPositive2())
    {
        this->shiftMSB(IBIT_0);
        return;
    }
    if(p_right.isNegative2())
    {
        this->shiftMSB(IBIT_0);
        this->negate();
        return;
    }

    /// comparison optimization
    // if()

    /// division:
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
    return number; /// copy elision aka RVO (Return Value Optimization)
    /// using move is "pessimising move" and after C++17 copy elision is guaranteed
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
    return number; /// copy elision aka RVO (Return Value Optimization)
    /// using move is "pessimising move" and after C++17 copy elision is guaranteed
}


