#pragma once

#include "InfiniterShared.hpp"
#include "InfiniterIO.hpp"

#include <string>
#include <vector>

template<typename InfiniterDerived>
class InfiniterUtility : public InfiniterIO<InfiniterDerived>
{
public:
    inline explicit InfiniterUtility() noexcept;

    inline InfiniterUtility(int64_t p_value); /// throws bad_alloc
    inline explicit InfiniterUtility(int64_t p_value, isize_t p_capacity); /// throws bad_alloc
    inline explicit InfiniterUtility(icell_t p_value, isize_t p_capacity, bool p_negative_value); /// throws bad_alloc
    inline explicit InfiniterUtility(const icell_t *p_array, isize_t p_size, bool p_negative_value=false); /// throws bad_alloc

    inline explicit InfiniterUtility(const std::string &p_number, int p_base=10, bool p_negative_value=false); /// throws bad_alloc /// InfiniterException::InvalidStringFormat
    inline explicit InfiniterUtility(const std::vector<uint8_t> &p_number, int p_base=10, bool p_negative_value=false); /// throws bad_alloc

    inline InfiniterUtility(const InfiniterDerived &p_source); /// throws bad_alloc
    inline InfiniterUtility(InfiniterDerived &&p_source) noexcept;

    inline ~InfiniterUtility() noexcept;

    /// InfiniterIO methods

    InfiniterDerived findGCD(const InfiniterDerived &p_number) const; /// Greatest Common Divisor
    InfiniterDerived findLCM(const InfiniterDerived &p_number) const; /// Least Common Multiple

    bool isPerfect() const; /// $$y(x) \approx e^\gamma \cdot x \cdot \ln(\ln(x))$$ $e^\gamma$ to stała (gdzie $\gamma$ to stała Eulera-Mascheroniego), wynosząca w przybliżeniu 1.781.
    bool isPrime() const;
    bool isHarshad() const; /// 18 is, 1+8=9, 18%9 == 9 /// reserve(log_10(m_size)+1)
    bool isFibonacci() const;

    std::vector<InfiniterDerived> getPrimeFactors() const;
    InfiniterDerived totient() const; ///(Funkcja Eulera) – zwraca liczbę liczb mniejszych od \(n\), które są z nią względnie pierwsze.
public:

    /// Operators

};

#include "InfiniterUtility.tpp"

