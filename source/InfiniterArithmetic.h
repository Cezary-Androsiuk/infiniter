#ifndef INFINITERARITHMETIC_H
#define INFINITERARITHMETIC_H

#include "InfiniterShared.hpp"
#include "InfiniterBit.h"

class InfiniterArithmetic : public InfiniterBit
{
public:
    explicit InfiniterArithmetic() noexcept;

    explicit InfiniterArithmetic(uint64_t p_capacity); /// throws bad_alloc
    explicit InfiniterArithmetic(uint64_t p_capacity, uint64_t p_value, bool p_negative_value=false); /// throws bad_alloc
    explicit InfiniterArithmetic(const cell_t *p_array, uint64_t p_size, bool p_negative_value=false); /// throws bad_alloc
};

#endif // INFINITERARITHMETIC_H
