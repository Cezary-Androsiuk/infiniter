#ifndef INFINITERMEMORY_H
#define INFINITERMEMORY_H

#include <cstdint>

#include "InfiniterCommon.hpp"

class InfiniterMemory
{
protected:
    /// Constructor
    explicit InfiniterMemory() noexcept;
    explicit InfiniterMemory(uint64_t p_capacity); /// throws bad_alloc

    /// Copy Constructor
    explicit InfiniterMemory(const InfiniterMemory &p_source); /// throws bad_alloc

    /// Move Constructor
    explicit InfiniterMemory(InfiniterMemory &&p_source) noexcept;

    ~InfiniterMemory() noexcept; /// NOT VIRTUAL

    void reset() noexcept;

    void reserve(uint64_t p_new_capacity); /// throws bad_alloc // in upper classes operator uint64_t() will be implemented, then Infiniter can be converted to scalars
    void reserve(const InfiniterMemory &p_source); /// throws bad_alloc
    void extend(uint64_t p_additional_capacity); /// throws bad_alloc
    void shrink(uint64_t p_target_capacity); /// throws bad_alloc

public:
#if IM_ENABLE_DB_PRINT_METHOD
    void dbg_print() const;
#define im_dbg_print(obj) obj.dbg_print()
#else // IM_ENABLE_DB_PRINT_METHOD
#define im_dbg_print(obj) printf("im_dbg_print disabled\n");
#endif // IM_ENABLE_DB_PRINT_METHOD
protected:

    /// Getters
    bool isSBOActive() const;

    /// Operators
    InfiniterMemory &operator =(const InfiniterMemory &p_source);
    InfiniterMemory &operator =(InfiniterMemory &&p_source);

protected:
    cell_t *m_memory;
    uint64_t m_capacity;

    BitsUS m_bits;

private:
    cell_t m_sbo_buffer[SBO_CAPACITY];

    // heap memory is allocated directly on m_memory

    friend void infiniterMemoryTests();
};

#endif // INFINITERMEMORY_H
