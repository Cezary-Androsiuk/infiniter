#ifndef INFINITERCORE_H
#define INFINITERCORE_H

#include <cstdint>

#include "InfiniterCommon.hpp"
#include "InfiniterMemory.h"

class InfiniterCore : private InfiniterMemory
{
public:
    /// Constructor
    explicit InfiniterCore() noexcept;

    InfiniterCore(uint64_t p_capacity); /// throws bad_alloc

    /// value only covers first cell, purpose of this is to initialize instance with 1 or something like that
    InfiniterCore(uint64_t p_capacity, uint64_t p_value, bool p_negative_value=false); /// throws bad_alloc
    // czy opłaca się oznaczać konstruktory z uint64_t jako explicit?
    // tak samo w IM

    /// Copy Constructor
    explicit InfiniterCore(const InfiniterCore &p_source); /// throws bad_alloc

    /// Move Constructor
    explicit InfiniterCore(InfiniterCore &&p_source) noexcept;





    /// Operators
    InfiniterCore &operator =(const InfiniterCore &p_source);
    InfiniterCore &operator =(InfiniterCore &&p_source);

private:


};

#endif // INFINITERCORE_H
