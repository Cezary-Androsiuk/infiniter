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
    void incrementMagnitude();
    void decrementMagnitude();

    void addMagnitude(icell_t p_value);
    void subtractMagnitude(icell_t p_value);

    void addMagnitude(const InfiniterDerived &p_number);
    void subtractMagnitude(const InfiniterDerived &p_number);

public:
    void increment();
    void decrement();

    void add(icell_t p_value, bool p_negative_value=false);
    void subtract(icell_t p_value, bool p_negative_value=false);

    void add(const InfiniterDerived &p_number);
    void subtract(const InfiniterDerived &p_number);

    void multiply(const InfiniterDerived &p_number);

    void divde(const InfiniterDerived &p_number);


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
