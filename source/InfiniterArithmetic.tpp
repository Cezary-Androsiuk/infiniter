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
InfiniterArithmetic<InfiniterDerived>::InfiniterArithmetic(int64_t p_value)
    : InfiniterBit<InfiniterDerived>(p_value)
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed   PARAMETER int64_t\n", this);
}

template<typename InfiniterDerived>
InfiniterArithmetic<InfiniterDerived>::InfiniterArithmetic(int64_t p_value, isize_t p_capacity)
    : InfiniterBit<InfiniterDerived>(p_value, p_capacity)
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed   PARAMETER int64_t isize_t\n", this);
}

template<typename InfiniterDerived>
InfiniterArithmetic<InfiniterDerived>::InfiniterArithmetic(icell_t p_value, isize_t p_capacity, bool p_negative_value)
    : InfiniterBit<InfiniterDerived>(p_value, p_capacity, p_negative_value)
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed   PARAMETER icell_t isize_t bool\n", this);
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
    return this->getRef();
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
    return this->getRef();
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
    return this->getRef();
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::subtractMagnitude(icell_t p_right)
{
    icell_t *data = this->getData();
    isize_t size = this->getRealSize();

    icell_t carry = p_right;
    for(isize_t i=0; i<size && carry; i++)
    {
        icell2_t sum = static_cast<icell2_t>(data[i]) + carry;
        data[i] = static_cast<icell_t>(sum);
        carry = static_cast<icell_t>(sum >> icell_bits);
    }

    if(carry)
    {
        size = this->setSizeWithExtend(size+1);
        this->getData()[size-1] = carry;
    }

    this->normalize();
    return this->getRef();
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
        icell2_t sum = static_cast<icell2_t>(ldata[i]) + rdata[i] + carry;

        ldata[i] = static_cast<icell_t>(sum);
        carry = static_cast<icell_t>(sum >> icell_bits);
    }

    /// move the rest (with carry)
    for(isize_t i=rsize; i<lsize && carry; i++)
    {
        icell2_t sum = static_cast<icell2_t>(ldata[i]) + carry;

        ldata[i] = static_cast<icell_t>(sum);
        carry = static_cast<icell_t>(sum >> icell_bits);
    }

    this->normalize();
    return this->getRef();
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
        icell2s_t diff = static_cast<icell2s_t>(ldata[i]) - rdata[i] - borrow;

        ldata[i] = static_cast<icell_t>(diff);
        borrow = static_cast<icell_t>(static_cast<icell2_t>(diff) >> (icell_bits*2 -1));
    }

    /// propagate borrow (move the rest)
    for(isize_t i = rsize; i < lsize && borrow; i++)
    {
        icell2s_t diff = static_cast<icell2s_t>(ldata[i]) - borrow;

        ldata[i] = static_cast<icell_t>(diff);
        borrow = static_cast<icell_t>(static_cast<icell2_t>(diff) >> (icell_bits*2 -1));
    }

    this->normalize();
    return this->getRef();
}

template<typename InfiniterDerived>
inline InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::setSignProduct(ibit_t p_left_sign, ibit_t p_right_sign)
{
    ibit_t sign_product = p_left_sign ^ p_right_sign;
    this->setSign(sign_product);
    return this->getRef();
}

template<typename InfiniterDerived>
inline void InfiniterArithmetic<InfiniterDerived>::split(
    const InfiniterDerived &p_input,
    InfiniterDerived &p_out_left, InfiniterDerived &p_out_right,
    isize_t p_half_size)
{
    const isize_t input_size = p_input.getSize();

    if(input_size <= p_half_size)
    {
        p_out_left.assign(0);
        p_out_right.assign(p_input);
        return;
    }

    const icell_t *input_data = p_input.getData();

    p_out_left.assign(input_data, p_half_size);
    p_out_right.assign(input_data + p_half_size, input_size - p_half_size);
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
    /// pre addition conditions
    if(p_right == 0)
    {
        this->normalize();
        return this->getRef();
    }
    if(p_right == 1)
    {
        if(p_negative_value)
            return this->decrement();  /// has normalize
        else
            return this->increment();  /// has normalize
    }
    /// -1 + -1   ->   -(1+1)
    /// -1 +  1   ->   -(1-1)
    ///  1 +  1   ->    (1+1)
    ///  1 + -1   ->    (1-1)
    ///
    /// -2 + -1   ->   -(2+1)
    /// -2 +  1   ->   -(2-1)
    ///  2 +  1   ->    (2+1)
    ///  2 + -1   ->    (2-1)
    ///
    /// -1 + -2   ->   -(1+2)
    /// -1 +  2   ->   -(1-2)   ->    (2-1)
    ///  1 +  2   ->    (1+2)
    ///  1 + -2   ->    (1-2)   ->   -(2-1)

    /// addition:
    /// left is negative
    if(this->getSign())
    {
        /// right is negative
        if(p_negative_value)
        {
            this->setNegativeSign();
            return this->addMagnitude(p_right);  /// has normalize
        }
        
        /// right is positive but left is smaller
        if(this->smallerMagnitude(p_right))
        {
            InfiniterDerived tmp_swap(p_right);
            tmp_swap.subtractMagnitude(this->getCRef()); /// has normalize
            tmp_swap.setPositiveSign();
            return this->assign(std::move(tmp_swap)); /// has normalize
        }

        /// right is positive
        this->setNegativeSign();
        return this->subtractMagnitude(p_right);  /// has normalize
    }
    /// left is positive
    else
    {
        /// right is positive
        if(!p_negative_value)
        {
            this->setPositiveSign();
            return this->addMagnitude(p_right);  /// has normalize
        }
        
        /// right is negative but left is smaller
        if(this->smallerMagnitude(p_right))
        {
            InfiniterDerived tmp_swap(p_right);
            tmp_swap.subtractMagnitude(this->getCRef()); /// has normalize
            tmp_swap.setNegativeSign();
            return this->assign(std::move(tmp_swap)); /// has normalize
        }

        /// right is negative
        this->setPositiveSign();
        return this->subtractMagnitude(p_right);  /// has normalize
    }


}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::add(const InfiniterDerived &p_right)
{
    /// pre addition conditions
    if(this->getRealSize() == 1)
    {
        /// just scalar
        bool sign = this->getSign();
        icell_t value = this->getData()[0];

        return this->assign(p_right)    /// has normalize
            .add(value, sign);          /// has normalize
    }

    if(p_right.getRealSize() == 1)
    {
        /// just scalar
        bool sign = p_right.getSign();
        icell_t value = p_right.getData()[0];

        return this->add(value, sign); /// has normalize
    }
    /// -1 + -1   ->   -(1+1)
    /// -1 +  1   ->   -(1-1)
    ///  1 +  1   ->    (1+1)
    ///  1 + -1   ->    (1-1)
    ///
    /// -2 + -1   ->   -(2+1)
    /// -2 +  1   ->   -(2-1)
    ///  2 +  1   ->    (2+1)
    ///  2 + -1   ->    (2-1)
    ///
    /// -1 + -2   ->   -(1+2)
    /// -1 +  2   ->   -(1-2)   ->    (2-1)
    ///  1 +  2   ->    (1+2)
    ///  1 + -2   ->    (1-2)   ->   -(2-1)

    /// addition:
    /// left is negative
    if(this->getSign())
    {
        /// right is negative
        if(p_right.getSign())
        {
            this->setNegativeSign();
            return this->addMagnitude(p_right);  /// has normalize
        }

        /// right is positive but left is smaller
        if(this->smallerMagnitude(p_right))
        {
            InfiniterDerived tmp_swap(p_right);  /// has normalize
            tmp_swap.addMagnitude(this->getCRef());  /// has normalize
            tmp_swap.setPositiveSign();
            return this->assign(std::move(tmp_swap));  /// has normalize
        }

        /// right is positive
        this->setNegativeSign();
        return this->subtractMagnitude(p_right);  /// has normalize
    }
    /// left is positive
    else
    {
        /// right is positive
        if(!p_right.getSign())
        {
            this->setPositiveSign();
            return this->addMagnitude(p_right);  /// has normalize
        }

        /// right is negative but left is smaller
        if(this->smallerMagnitude(p_right))
        {
            InfiniterDerived tmp_swap(p_right);  /// has normalize
            tmp_swap.subtractMagnitude(this->getCRef());  /// has normalize
            tmp_swap.setNegativeSign();
            return this->assign(std::move(tmp_swap));  /// has normalize
        }

        /// right is negative
        this->setPositiveSign();
        return this->subtractMagnitude(p_right);  /// has normalize
    }
}

template<typename InfiniterDerived>
inline InfiniterDerived InfiniterArithmetic<InfiniterDerived>::plus(icell_t p_right, bool p_negative_value) const
{
    InfiniterDerived result(this->getCRef());
    result.add(p_right, p_negative_value);
    return result; /// copy elision aka RVO (Return Value Optimization)
    /// using move is "pessimising move" and after C++17 copy elision is guaranteed
}

template<typename InfiniterDerived>
inline InfiniterDerived InfiniterArithmetic<InfiniterDerived>::plus(const InfiniterDerived &p_right) const
{
    InfiniterDerived result(this->getCRef());
    result.add(p_right);
    return result; /// copy elision aka RVO (Return Value Optimization)
    /// using move is "pessimising move" and after C++17 copy elision is guaranteed
}

template<typename InfiniterDerived>
inline InfiniterDerived InfiniterArithmetic<InfiniterDerived>::add(icell_t p_left, icell_t p_right, bool p_left_negative, bool p_right_negative)
{
    InfiniterDerived result(p_left, 1, p_left_negative);
    result.add(p_right, p_right_negative);
    return result; /// copy elision aka RVO (Return Value Optimization)
    /// using move is "pessimising move" and after C++17 copy elision is guaranteed
}

template<typename InfiniterDerived>
inline InfiniterDerived InfiniterArithmetic<InfiniterDerived>::add(const InfiniterDerived &p_left, const InfiniterDerived &p_right)
{
    return p_left.plus(p_right); /// copy elision aka RVO (Return Value Optimization)
    /// using move is "pessimising move" and after C++17 copy elision is guaranteed
}



template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::subtract(icell_t p_right, bool p_negative_value)
{
    /// pre subtraction conditions
    if(p_right == 0)
    {
        this->normalize();
        return this->getRef();
    }
    if(p_right == 1)
    {
        if(p_negative_value)
            return this->increment();  /// has normalize
        else
            return this->decrement();  /// has normalize
    }
    /// -1 -  1   ->   -(1+1)
    /// -1 - -1   ->   -(1-1)
    ///  1 -  1   ->    (1-1)
    ///  1 - -1   ->    (1+1)
    ///
    /// -2 -  1   ->   -(2+1)
    /// -2 - -1   ->   -(2-1)
    ///  2 - -1   ->    (2+1)
    ///  2 -  1   ->    (2-1)
    ///
    /// -1 -  2   ->   -(1+2)
    /// -1 - -2   ->   -(1-2)   ->    (2-1)
    ///  1 - -2   ->    (1+2)
    ///  1 -  2   ->    (1-2)   ->   -(2-1)

    /// subtraction:
    /// left is negative
    if(this->getSign())
    {
        /// right is positive
        if(!p_negative_value)
        {
            this->setNegativeSign();
            return this->addMagnitude(p_right);  /// has normalize
        }

        /// right is negative but left is smaller
        if(this->smallerMagnitude(p_right))
        {
            /// swap, subtract and set positive
            InfiniterDerived tmp_swap(p_right);  /// has normalize
            tmp_swap.subtractMagnitude(this->getCRef());  /// has normalize
            tmp_swap.setPositiveSign();
            return this->assign(std::move(tmp_swap));  /// has normalize
        }

        /// right is negative
        this->setNegativeSign();
        return this->subtractMagnitude(p_right);  /// has normalize
    }
    /// left is positive
    else
    {
        /// right is negative
        if(p_negative_value)
        {
            this->setPositiveSign();
            return this->addMagnitude(p_right);  /// has normalize
        }

        /// right is positive but left is smaller
        if(this->smallerMagnitude(p_right))
        {
            /// swap, add, set negative
            InfiniterDerived tmp_swap(p_right);  /// has normalize
            tmp_swap.addMagnitude(this->getCRef());  /// has normalize
            tmp_swap.setNegativeSign();
            return this->assign(std::move(tmp_swap));  /// has normalize
        }

        /// right is positive
        this->setPositiveSign();
        return this->subtractMagnitude(p_right);  /// has normalize
    }
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::subtract(const InfiniterDerived &p_right)
{
    /// pre subtraction conditions
    if(this->getRealSize() == 1)
    {
        /// just scalar
        bool sign = this->getSign();
        icell_t value = this->getData()[0];

        return this->assign(p_right)    /// has normalize
            .subtract(value, sign);     /// has normalize
    }
    if(p_right.getRealSize() == 1)
    {
        /// just scalar
        bool sign = p_right.getSign();
        icell_t value = p_right.getData()[0];

        return this->subtract(value, sign); /// has normalize
    }
    /// -1 -  1   ->   -(1+1)
    /// -1 - -1   ->   -(1-1)
    ///  1 -  1   ->    (1-1)
    ///  1 - -1   ->    (1+1)
    ///
    /// -2 -  1   ->   -(2+1)
    /// -2 - -1   ->   -(2-1)
    ///  2 - -1   ->    (2+1)
    ///  2 -  1   ->    (2-1)
    ///
    /// -1 -  2   ->   -(1+2)
    /// -1 - -2   ->   -(1-2)   ->    (2-1)
    ///  1 - -2   ->    (1+2)
    ///  1 -  2   ->    (1-2)   ->   -(2-1)

    /// subtraction:
    /// left is negative
    if(this->getSign())
    {
        /// right is positive
        if(!p_right.getSign())
        {
            this->setNegativeSign();
            return this->addMagnitude(p_right);  /// has normalize
        }

        /// right is negative but left is smaller
        if(this->smallerMagnitude(p_right))
        {
            InfiniterDerived tmp_swap(p_right);  /// has normalize
            tmp_swap.subtractMagnitude(this->getCRef());  /// has normalize
            tmp_swap.setPositiveSign();
            return this->assign(std::move(tmp_swap));  /// has normalize
        }

        /// right is negative
        this->setNegativeSign();
        return this->subtractMagnitude(p_right);  /// has normalize

    }
    /// left is positive
    else
    {
        /// right is negative
        if(p_right.getSign())
        {
            this->setPositiveSign();
            return this->addMagnitude(p_right);  /// has normalize
        }

        /// right is positive but left is smaller
        if(this->smallerMagnitude(p_right))
        {
            InfiniterDerived tmp_swap(p_right);  /// has normalize
            tmp_swap.addMagnitude(this->getCRef());  /// has normalize
            tmp_swap.setNegativeSign();
            return this->assign(std::move(tmp_swap));  /// has normalize
        }

        /// right is positive
        this->setPositiveSign();
        return this->subtractMagnitude(p_right);  /// has normalize
    }
}

template<typename InfiniterDerived>
inline InfiniterDerived InfiniterArithmetic<InfiniterDerived>::minus(icell_t p_right, bool p_negative_value) const
{
    InfiniterDerived result(this->getCRef());
    result.subtract(p_right, p_negative_value);
    return result; /// copy elision aka RVO (Return Value Optimization)
    /// using move is "pessimising move" and after C++17 copy elision is guaranteed
}

template<typename InfiniterDerived>
inline InfiniterDerived InfiniterArithmetic<InfiniterDerived>::minus(const InfiniterDerived &p_right) const
{
    InfiniterDerived result(this->getCRef());
    result.subtract(p_right);
    return result; /// copy elision aka RVO (Return Value Optimization)
    /// using move is "pessimising move" and after C++17 copy elision is guaranteed
}

template<typename InfiniterDerived>
inline InfiniterDerived InfiniterArithmetic<InfiniterDerived>::subtract(icell_t p_left, icell_t p_right, bool p_left_negative, bool p_right_negative)
{
    InfiniterDerived result(p_left, 1, p_left_negative);
    result.subtract(p_right, p_right_negative);
    return result; /// copy elision aka RVO (Return Value Optimization)
    /// using move is "pessimising move" and after C++17 copy elision is guaranteed
}

template<typename InfiniterDerived>
inline InfiniterDerived InfiniterArithmetic<InfiniterDerived>::subtract(const InfiniterDerived &p_left, const InfiniterDerived &p_right)
{
    return p_left.minus(p_right); /// copy elision aka RVO (Return Value Optimization)
    /// using move is "pessimising move" and after C++17 copy elision is guaranteed
}



template<typename InfiniterDerived>
inline InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::multiplyNaive(const InfiniterDerived &p_right)
{
    // return *this = IA::multiplyNaive(this->getCRef(), p_right));
    return this->assign(IA::multiplyNaive(this->getCRef(), p_right)); /// move is already active here
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::multiply(const InfiniterDerived &p_right)
{
    // return *this = IA::multiply(this->getCRef(), p_right));
    return this->assign(IA::multiply(this->getCRef(), p_right)); /// move is already active here
}

template<typename InfiniterDerived>
inline InfiniterDerived InfiniterArithmetic<InfiniterDerived>::timesNaive(const InfiniterDerived &p_right) const
{
    return IA::multiplyNaive(this->getCRef(), p_right);/// copy elision aka RVO (Return Value Optimization)
    /// using move is "pessimising move" and after C++17 copy elision is guaranteed
}

template<typename InfiniterDerived>
InfiniterDerived InfiniterArithmetic<InfiniterDerived>::times(const InfiniterDerived &p_right) const
{
    return IA::multiply(this->getCRef(), p_right);/// copy elision aka RVO (Return Value Optimization)
    /// using move is "pessimising move" and after C++17 copy elision is guaranteed
}

template<typename InfiniterDerived>
inline InfiniterDerived InfiniterArithmetic<InfiniterDerived>::multiplyNaive(const InfiniterDerived &p_left,
                                                                             const InfiniterDerived &p_right)
{
    /// pre multiplication conditions
    isize_t left_size = p_left.getRealSize();
    isize_t right_size = p_right.getRealSize();

    if(left_size == 1)
    {
        bool sign = p_left.getSign();
        icell_t value = p_left.getData()[0];

        /// handle edge cases p_left
        if(value == 0)
        {
            return InfiniterDerived();
        }
        if(value == 1)
        {
            const icell_t *data = p_right.getData();
            const isize_t size = p_right.getSize();
            const bool sign = p_left.getSign() ^ p_right.getSign();

            return InfiniterDerived(data, size, sign);   /// has normalize
        }
        if(value == 2)
        {
            const icell_t *data = p_right.getData();
            const isize_t size = p_right.getSize();
            const bool sign = p_left.getSign() ^ p_right.getSign();

            return InfiniterDerived(data, size, sign)   /// has normalize
                .shiftLeft();                           /// has normalize
        }
    }

    if(right_size == 1)
    {
        bool sign = p_right.getSign();
        icell_t value = p_right.getData()[0];

        /// handle edge cases p_right
        if(p_right.is0())
        {
            return InfiniterDerived();
        }
        if(p_right.is1())
        {
            const icell_t *data = p_left.getData();
            const isize_t size = p_left.getSize();
            const bool sign = p_left.getSign() ^ p_right.getSign();

            return InfiniterDerived(data, size, sign);   /// has normalize
        }
        if(p_right.is2())
        {
            const icell_t *data = p_left.getData();
            const isize_t size = p_left.getSize();
            const bool sign = p_left.getSign() ^ p_right.getSign();

            return InfiniterDerived(data, size, sign)   /// has normalize
                .shiftLeft();                           /// has normalize
        }
    }

    isize_t result_size = left_size + right_size;
    bool result_sign = p_left.getSign() ^ p_right.getSign();

    InfiniterDerived result(ICELL_C(0), result_size, result_sign);
    result.setSize(result_size);

    const icell_t *left_data = p_left.getData();
    const icell_t *right_data = p_right.getData();
    icell_t *result_data = result.getData();

    for(isize_t j=0; j<left_size; j++)
    {
        icell_t carry = 0;

        for(isize_t i=0; i<right_size; i++)
        {
            icell2_t raw_result = static_cast<icell2_t>(left_data[j]) * right_data[i] +
                                     result_data[j + i] + carry;

            result_data[j + i] = static_cast<icell_t>(raw_result);

            carry = static_cast<icell_t>(raw_result >> icell_bits);
        }

        if(carry > ICELL_C(0))
        {
            result_data[j + right_size] += carry;
        }
    }

    result.normalize();
    return result; /// copy elision aka RVO (Return Value Optimization)
    /// using move is "pessimising move" and after C++17 copy elision is guaranteed
}

template<typename InfiniterDerived>
inline InfiniterDerived InfiniterArithmetic<InfiniterDerived>::multiply(const InfiniterDerived &p_left,
                                                                        const InfiniterDerived &p_right)
{
    isize_t max_size = std::max(p_left.getRealSize(), p_right.getRealSize());

    /// multiplication naive
    /// for smaller numbers is faster
    if(max_size <= 32)
    {
        return IA::multiplyNaive(p_left, p_right);   /// has normalize
    }

    /// karatsuba
    isize_t half_size = max_size / 2;

    InfiniterDerived left_lhalf, left_rhalf;  /// left  left half | left  right half
    InfiniterDerived right_lhalf, right_rhalf;  /// right left half | right right half

    IA::split(p_left, left_lhalf, left_rhalf, half_size);
    IA::split(p_right, right_lhalf, right_rhalf, half_size);

    InfiniterDerived result_part_2 = IA::multiply(left_lhalf, right_lhalf);
    InfiniterDerived result_part_0 = IA::multiply(left_rhalf, right_rhalf);

    /// use .add() and .subtract() on the original object to prevent copying objects
    /// (llh + lrh) * (rlh + rrh) - rp2 - rp0
    InfiniterDerived result_part_1 = IA::multiply(
                                         left_lhalf.add(left_rhalf),
                                         right_lhalf.add(right_rhalf)
                                         )
                                         .subtract(result_part_2)
                                         .subtract(result_part_0);

    /// again use use .shiftLeft() and .add() on the original object to prevent copying objects
    /// (rp2 * 2^(2 * hs)) + (rp1 * 2^hs) + rp0
    return result_part_2.shiftLeft(2*half_size)
        .add(result_part_1.shiftLeft(half_size))
        .add(result_part_0);
}



template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::divde(const InfiniterDerived &p_right)
{

}

template<typename InfiniterDerived>
inline typename InfiniterArithmetic<InfiniterDerived>::InfiniterDivisionResult
InfiniterArithmetic<InfiniterDerived>::divdeRem(const InfiniterDerived &p_right)
{

}

template<typename InfiniterDerived>
inline InfiniterDerived InfiniterArithmetic<InfiniterDerived>::over(const InfiniterDerived &p_right) const
{

}

template<typename InfiniterDerived>
inline typename InfiniterArithmetic<InfiniterDerived>::InfiniterDivisionResult
InfiniterArithmetic<InfiniterDerived>::overRem(const InfiniterDerived &p_right)
{

}

template<typename InfiniterDerived>
inline InfiniterDerived InfiniterArithmetic<InfiniterDerived>::divde(const InfiniterDerived &p_left, const InfiniterDerived &p_right)
{
    isize_t left_size = p_left.getRealSize();
    isize_t right_size = p_right.getRealSize();

    if(right_size == 1)
    {
        bool sign = p_right.getSign();
        icell_t value = p_right.getData()[0];

        if(value == 0)
        {
            throw InfiniterException::ZeroDivision();
        }

        /// standard division
        if(left_size == 1)
        {
            InfiniterDerived result(
                p_left.getData()[0] / value, 1,
                p_left.getSign() ^ sign);
            return result;
        }

        /// handle easy cases like:
        /// 000...001
        /// 000...010
        /// ...
        /// 100...000
        bool right_is_pow_2 = value && !(value & (value -1));
        if(right_is_pow_2)
        {
            InfiniterDerived tmp_left(p_left);
            return tmp_left.shiftRight(ICELL_LSB_POS(value));
        }

    }

    /// x / x+y    where y > 0, is always 0
    if(left_size < right_size) // or full comparison
    {
        return InfiniterDerived();
    }

    bool sign = p_left.getSign() ^ p_right.getSign();
    InfiniterDerived result(0, left_size, sign);

    /// comparison optimization
    for(isize_t i=0; i<left_size; i++)
    {
        isize_t i_rev = left_size -i -1;

        result.shiftLeft(1);


    }

    // if()
    // // 1. Obsługa błędu krytycznego
    // if (y.blocks.empty() || (y.blocks.size() == 1 && y.blocks[0] == 0)) {
    //     throw std::domain_error("Division by zero");
    // }

    // DivModResult result;
    // result.quotient.blocks.assign(x.blocks.size(), 0);
    // result.remainder.blocks = {0};

    // // 2. Optymalizacja: Jeśli |X| < |Y|, wynik to 0, a reszta to X.
    // if (abs_compare(x, y) < 0) {
    //     result.quotient.blocks = {0};
    //     result.remainder = x;
    //     return result;
    // }

    // // 3. Główna pętla dzielenia bitowego.
    // // Iterujemy od najbardziej znaczącego bitu dzielnej (X) w dół do zera.
    // // Metoda total_bits() powinna zwracać pozycję najwyższego ustawionego bitu.
    // int max_bit = x.total_bits() - 1;

    // for (int i = max_bit; i >= 0; --i) {
    //     // A) Przesuń resztę o 1 bit w lewo (mnożenie przez 2)
    //     result.remainder.shift_left_by_one();

    //     // B) Pobierz i-ty bit z X i wstaw na najmłodszą pozycję reszty
    //     if (x.test_bit(i)) {
    //         result.remainder.set_bit(0, 1);
    //     }

    //     // C) Sprawdź, czy reszta jest większa lub równa dzielnikowi (na modułach)
    //     if (abs_compare(result.remainder, y) >= 0) {
    //         // Odejmowanie wartości bezwzględnych
    //         result.remainder = abs_subtract(result.remainder, y);

    //         // Ustawiamy i-ty bit w ilorazie na 1
    //         result.quotient.set_bit(i, 1);
    //     }
    // }

    // // 4. Czyszczenie wiodących zer w ilorazie
    // result.quotient.trim_leading_zeros();
    // result.remainder.trim_leading_zeros();

    // // 5. Ustalanie znaków
    // // Iloraz ma znak ujemny tylko wtedy, gdy operandy miały różne znaki.
    // result.quotient.is_negative = (x.is_negative != y.is_negative) && !result.quotient.is_zero();

    // // Znak reszty w matematyce zależy od konwencji. W C++ (od C++11)
    // // operator % dla typów wbudowanych zachowuje znak dzielnej (X).
    // result.remainder.is_negative = x.is_negative && !result.remainder.is_zero();

    // return result;

    /// division:
}

template<typename InfiniterDerived>
inline typename InfiniterArithmetic<InfiniterDerived>::InfiniterDivisionResult
InfiniterArithmetic<InfiniterDerived>::divdeRem(const InfiniterDerived &p_left, const InfiniterDerived &p_right)
{

}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::operator ++()
{
    return this->increment();
}

template<typename InfiniterDerived>
InfiniterDerived InfiniterArithmetic<InfiniterDerived>::operator ++(int)
{
    InfiniterDerived number(this->getCRef()); /// static cast needed?
    this->increment();
    return number; /// copy elision aka RVO (Return Value Optimization)
    /// using move is "pessimising move" and after C++17 copy elision is guaranteed
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterArithmetic<InfiniterDerived>::operator --()
{
    return this->decrement();
}

template<typename InfiniterDerived>
InfiniterDerived InfiniterArithmetic<InfiniterDerived>::operator --(int)
{
    InfiniterDerived number(this->getCRef()); /// static cast needed?
    this->decrement();
    return number; /// copy elision aka RVO (Return Value Optimization)
    /// using move is "pessimising move" and after C++17 copy elision is guaranteed
}


