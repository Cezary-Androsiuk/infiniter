#pragma once

#include "InfiniterShared.hpp"
#include "InfiniterBit.hpp"

template<typename InfiniterDerived>
class InfiniterArithmetic : public InfiniterBit<InfiniterDerived>
{
public:
    inline explicit InfiniterArithmetic() noexcept;

    inline explicit InfiniterArithmetic(isize_t p_capacity); /// throws bad_alloc
    inline explicit InfiniterArithmetic(isize_t p_capacity, icell_t p_value, bool p_negative_value=false); /// throws bad_alloc
    inline explicit InfiniterArithmetic(const icell_t *p_array, isize_t p_size, bool p_negative_value=false); /// throws bad_alloc

    inline InfiniterArithmetic(const InfiniterDerived &p_source); /// throws bad_alloc
    inline InfiniterArithmetic(InfiniterDerived &&p_source) noexcept;

    inline ~InfiniterArithmetic() noexcept;

    /// InfiniterBit methods

protected:
    InfiniterDerived &incrementMagnitude();
    InfiniterDerived &decrementMagnitude();

    InfiniterDerived &addMagnitude(icell_t p_right);
    InfiniterDerived &subtractMagnitude(icell_t p_right);

    InfiniterDerived &addMagnitude(const InfiniterDerived &p_right);
    InfiniterDerived &subtractMagnitude(const InfiniterDerived &p_right);

public:
    InfiniterDerived &increment();
    InfiniterDerived &decrement();


    
    InfiniterDerived &add(icell_t p_right, bool p_negative_value=false);
    InfiniterDerived &subtract(icell_t p_right, bool p_negative_value=false);

    InfiniterDerived &add(const InfiniterDerived &p_right);
    InfiniterDerived &subtract(const InfiniterDerived &p_right);

    InfiniterDerived plus(icell_t p_right, bool p_negative_value=false);
    InfiniterDerived subtract(icell_t p_right, bool p_negative_value=false);

    InfiniterDerived plus(const InfiniterDerived &p_right);
    InfiniterDerived subtract(const InfiniterDerived &p_right);



    InfiniterDerived &multiply(const InfiniterDerived &p_right);

    InfiniterDerived &divde(const InfiniterDerived &p_right);


public:

    /// Operators
    inline InfiniterDerived &operator ++ ();    /// ++a
    inline InfiniterDerived operator ++ (int);  /// a++
    inline InfiniterDerived &operator -- ();    /// --a
    inline InfiniterDerived operator -- (int);  /// a--

    /// addition
    /// subraction
    /// multiplication
    /// division
    /// modulo
    /// exponent
    /// sqrt


};

#include "InfiniterArithmetic.tpp"
