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

    // maybe move to ICore
    bool isNumber(icell_t p_scalar, int p_sign) const noexcept;

    bool is0() const noexcept;
    bool isPositive1() const noexcept;
    bool isNegative1() const noexcept;
    bool isPositive2() const noexcept;
    bool isNegative2() const noexcept;

    void normalize() noexcept;

    Infiniter absoluteValue() const;
    void absoluteValueAssign();

protected:
    void incrementMagnitude();
    void decrementMagnitude();

    void addMagnitude(icell_t p_value);
    void subtractMagnitude(icell_t p_value);

    void addMagnitude(const Infiniter &p_number);
    void subtractMagnitude(const Infiniter &p_number);

public:
    void increment();
    void decrement();

    void add(icell_t p_value, bool p_negative_value=false);
    void subtract(icell_t p_value, bool p_negative_value=false);

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
