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

    explicit InfiniterCore(uint64_t p_capacity); /// throws bad_alloc

    /// value only covers first cell, purpose of this is to initialize instance with 1 or something like that
    explicit InfiniterCore(uint64_t p_capacity, uint64_t p_value, bool p_negative_value=false); /// throws bad_alloc
    // czy opłaca się oznaczać konstruktory z uint64_t jako explicit?
    // tak samo w IM

    /// Copy Constructor
    explicit InfiniterCore(const InfiniterCore &p_source); /// throws bad_alloc

    /// Move Constructor
    explicit InfiniterCore(InfiniterCore &&p_source) noexcept;

    ~InfiniterCore() noexcept;

    void reset() noexcept;

    void reserve(uint64_t p_new_capacity);
    void reserve(const InfiniterCore &p_source);
    void extend(uint64_t p_additional_capacity);
    void shrink();

    cell_t *getData() noexcept;
    const cell_t *getData() const noexcept;
    uint64_t getSize() const noexcept;
    uint64_t getCapacity() const noexcept;

    uint8_t getSign() const noexcept;

public:
#if IC_ENABLE_DB_PRINT_METHOD
    void dbg_print() const;
#define ic_dbg_print(obj) obj.dbg_print()
#else // IC_ENABLE_DB_PRINT_METHOD
#define ic_dbg_print(obj) printf("ic_dbg_print disabled\n");
#endif // IC_ENABLE_DB_PRINT_METHOD
public:

    /// Operators
    InfiniterCore &operator =(const InfiniterCore &p_source);
    InfiniterCore &operator =(InfiniterCore &&p_source);

protected:
    uint64_t m_size;

    friend void infiniterCoreTests();
};

#endif // INFINITERCORE_H
