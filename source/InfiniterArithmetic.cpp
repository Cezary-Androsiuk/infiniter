#include "InfiniterArithmetic.hpp"


InfiniterArithmetic::InfiniterArithmetic() noexcept
    : InfiniterBit()
{

}

InfiniterArithmetic::InfiniterArithmetic(uint64_t p_capacity)
    : InfiniterBit(p_capacity)
{

}

InfiniterArithmetic::InfiniterArithmetic(uint64_t p_capacity, uint64_t p_value, bool p_negative_value)
    : InfiniterBit(p_capacity, p_value, p_negative_value)

{

}

InfiniterArithmetic::InfiniterArithmetic(const cell_t *p_array, uint64_t p_size, bool p_negative_value)
    : InfiniterBit(p_array, p_size, p_negative_value)

{

}
