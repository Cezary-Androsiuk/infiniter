#pragma once

#include "InfiniterShared.hpp"
#include "InfiniterBit.hpp"

struct InfiniterDivisionResult
{

};

template<typename InfiniterDerived>
class InfiniterArithmetic : public InfiniterBit<InfiniterDerived>
{
public:
    struct InfiniterDivisionResult
    {
        InfiniterDerived result;
        InfiniterDerived remainder;
    };

protected:
    using IA = InfiniterArithmetic<InfiniterDerived>;

public:
    inline explicit InfiniterArithmetic() noexcept;

    inline InfiniterArithmetic(int64_t p_value); /// throws bad_alloc
    inline explicit InfiniterArithmetic(int64_t p_value, isize_t p_capacity); /// throws bad_alloc
    inline explicit InfiniterArithmetic(icell_t p_value, isize_t p_capacity, bool p_negative_value); /// throws bad_alloc
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

    InfiniterDerived &setSignProduct(ibit_t p_left_sign, ibit_t p_right_sign);
    static void split(const InfiniterDerived &p_input,
                      InfiniterDerived &p_out_left, InfiniterDerived &p_out_right,
                      isize_t p_half_size);

public:
    InfiniterDerived &increment();
    InfiniterDerived &decrement();


    
    InfiniterDerived &add(icell_t p_right, bool p_negative_value=false);
    InfiniterDerived &add(const InfiniterDerived &p_right);

    InfiniterDerived plus(icell_t p_right, bool p_negative_value=false) const;
    InfiniterDerived plus(const InfiniterDerived &p_right) const;

    static InfiniterDerived add(icell_t p_left, icell_t p_right,
                                bool p_left_negative=false,
                                bool p_right_negative=false);
    static InfiniterDerived add(const InfiniterDerived &p_left,
                                const InfiniterDerived &p_right);



    InfiniterDerived &subtract(icell_t p_right, bool p_negative_value=false);
    InfiniterDerived &subtract(const InfiniterDerived &p_right);

    InfiniterDerived minus(icell_t p_right, bool p_negative_value=false) const;
    InfiniterDerived minus(const InfiniterDerived &p_right) const;

    static InfiniterDerived subtract(icell_t p_left, icell_t p_right,
                                     bool p_left_negative=false,
                                     bool p_right_negative=false);
    static InfiniterDerived subtract(const InfiniterDerived &p_left,
                                     const InfiniterDerived &p_right);



    InfiniterDerived &multiplyNaive(const InfiniterDerived &p_right);
    InfiniterDerived &multiply(const InfiniterDerived &p_right);

    InfiniterDerived timesNaive(const InfiniterDerived &p_right) const;
    InfiniterDerived times(const InfiniterDerived &p_right) const;

    static InfiniterDerived multiplyNaive(const InfiniterDerived &p_left,
                                          const InfiniterDerived &p_right);
    static InfiniterDerived multiply(const InfiniterDerived &p_left,
                                     const InfiniterDerived &p_right);



    InfiniterDerived &divde(const InfiniterDerived &p_right);
    InfiniterDivisionResult divdeRem(const InfiniterDerived &p_right);

    InfiniterDerived over(const InfiniterDerived &p_right) const;
    InfiniterDivisionResult overRem(const InfiniterDerived &p_right);

    static InfiniterDerived divde(const InfiniterDerived &p_left,
                                  const InfiniterDerived &p_right);
    InfiniterDivisionResult divdeRem(const InfiniterDerived &p_left,
                                    const InfiniterDerived &p_right);


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
