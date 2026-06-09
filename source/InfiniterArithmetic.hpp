#ifndef INFINITERARITHMETIC_HPP
#define INFINITERARITHMETIC_HPP

#include "InfiniterShared.hpp"
#include "InfiniterBit.hpp"

class Infiniter;

class InfiniterArithmetic : public InfiniterBit
{
public:
    explicit InfiniterArithmetic() noexcept;

    explicit InfiniterArithmetic(isize_t p_capacity); /// throws bad_alloc
    explicit InfiniterArithmetic(isize_t p_capacity, icell_t p_value, bool p_negative_value=false); /// throws bad_alloc
    explicit InfiniterArithmetic(const icell_t *p_array, isize_t p_size, bool p_negative_value=false); /// throws bad_alloc

    InfiniterArithmetic(const InfiniterArithmetic &p_source); /// throws bad_alloc
    InfiniterArithmetic(InfiniterArithmetic &&p_source) noexcept;

    InfiniterArithmetic(const InfiniterBit &p_source); /// throws bad_alloc
    InfiniterArithmetic(InfiniterBit &&p_source) noexcept;

    InfiniterArithmetic(const InfiniterCore &p_source); /// throws bad_alloc
    InfiniterArithmetic(InfiniterCore &&p_source) noexcept;

    ~InfiniterArithmetic() noexcept;

    /// InfiniterBit methods

    bool is0() const noexcept;
    bool isPositive1() const noexcept;
    bool isNegative1() const noexcept;
    bool isPositive2() const noexcept;
    bool isNegative2() const noexcept;

    Infiniter absoluteValue() const;
    void absoluteValueAssign();

    void increment();
    void decrement();

protected:
    void addScalarABS(icell_t p_value);
    void subtractScalarABS(icell_t p_value);

public:
    void addScalar(icell_t p_value, bool p_negative_value=false);
    void subtractScalar(icell_t p_value, bool p_negative_value=false);

    void add(const Infiniter &p_number);
    void subtract(const Infiniter &p_number);

    void multiply(const Infiniter &p_number);

    void divde(const Infiniter &p_number);

    using InfiniterBit::assign;
    void assign(const InfiniterArithmetic &p_source);
    void assign(InfiniterArithmetic &&p_source);

public:

    /// Operators
    using InfiniterBit::operator = ;
    InfiniterArithmetic &operator = (const InfiniterArithmetic &p_source);
    InfiniterArithmetic &operator = (InfiniterArithmetic &&p_source);
    InfiniterArithmetic &operator ++ ();    /// ++a
    InfiniterArithmetic operator ++ (int);  /// a++
    InfiniterArithmetic &operator -- ();    /// --a
    InfiniterArithmetic operator -- (int);  /// a-- // move to infiniter


};

#endif // INFINITERARITHMETIC_HPP
