#ifndef INFINITERUTILITY_HPP
#define INFINITERUTILITY_HPP

#include "InfiniterShared.hpp"
#include "InfiniterArithmetic.hpp"

class InfiniterUtility : public InfiniterArithmetic
{
public:
    explicit InfiniterUtility() noexcept;

    explicit InfiniterUtility(uint64_t p_capacity); /// throws bad_alloc
    explicit InfiniterUtility(uint64_t p_capacity, uint64_t p_value, bool p_negative_value=false); /// throws bad_alloc
    explicit InfiniterUtility(const cell_t *p_array, uint64_t p_size, bool p_negative_value=false); /// throws bad_alloc

    explicit InfiniterUtility(const InfiniterUtility &p_source); /// throws bad_alloc
    explicit InfiniterUtility(InfiniterUtility &&p_source) noexcept;

    ~InfiniterUtility() noexcept;

    /// InfiniterArithmetic methods

};

#endif // INFINITERUTILITY_HPP
