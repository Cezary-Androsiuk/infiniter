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
    InfiniterCore(uint64_t p_capacity, uint64_t p_value, bool p_positive_value=true); /// throws bad_alloc
    // czy opłaca się oznaczać konstruktory z uint64_t jako explicit?
    // tak samo w IM

    /// Copy Constructor
    explicit InfiniterCore(const InfiniterCore &p_source); /// throws bad_alloc

    /// Move Constructor
    explicit InfiniterCore(InfiniterMemory &&p_source) noexcept;


private:


};

#endif // INFINITERCORE_H
