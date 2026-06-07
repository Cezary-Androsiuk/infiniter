#ifndef INFINITERARITHMETIC_HPP
#define INFINITERARITHMETIC_HPP

#include "InfiniterShared.hpp"
#include "InfiniterBit.hpp"

class InfiniterArithmetic : public InfiniterBit
{
public:
    explicit InfiniterArithmetic() noexcept;

    explicit InfiniterArithmetic(isize_t p_capacity); /// throws bad_alloc
    explicit InfiniterArithmetic(isize_t p_capacity, icell_t p_value, bool p_negative_value=false); /// throws bad_alloc
    explicit InfiniterArithmetic(const icell_t *p_array, isize_t p_size, bool p_negative_value=false); /// throws bad_alloc

    explicit InfiniterArithmetic(const InfiniterArithmetic &p_source); /// throws bad_alloc
    explicit InfiniterArithmetic(InfiniterArithmetic &&p_source) noexcept;

    ~InfiniterArithmetic() noexcept;

    /// InfiniterBit methods


};

#endif // INFINITERARITHMETIC_HPP
