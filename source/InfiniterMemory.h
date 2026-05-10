#ifndef INFINITERMEMORY_H
#define INFINITERMEMORY_H

#include <cstdint>

#include "InfiniterCommon.hpp"

/// prints what is happening - what methods are executed
#define INFINITER_MEMORY_DEBUG_PRINT true

// template <uint64_t SBO_CAPACITY = DEFAULT_SBO_CAPACITY> // possible but another leayer of complexity and makes code hard to implement/read
class InfiniterMemory
{
protected: public:
    /// Constructor
    explicit InfiniterMemory() noexcept;
    explicit InfiniterMemory(uint64_t p_capacity); /// throws bad_alloc

    /// Copy Constructor
    explicit InfiniterMemory(const InfiniterMemory &p_source); /// throws bad_alloc

    /// Move Constructor
    explicit InfiniterMemory(InfiniterMemory &&p_source) noexcept;

    ~InfiniterMemory();


    void reserve(uint64_t p_new_capacity); /// throws bad_alloc // in upper classes operator uint64_t() will be implemented, then Infiniter can be converted to scalars
    void reserve(const InfiniterMemory &p_source); /// throws bad_alloc
    void extend(uint64_t p_additional_capacity); /// throws bad_alloc

    /// might require m_size update in InfiniterCore
    void shrink(); /// throws bad_alloc
    void shrink(uint64_t p_target_capacity); /// throws bad_alloc

    void reset() noexcept;

    void dbg_print() const;

    /// Getters
    bool isSBOActive() const;

    /// Operators
    InfiniterMemory &operator =(const InfiniterMemory &p_source);
    InfiniterMemory &operator =(InfiniterMemory &&p_source);

protected: public:
    cell_t *m_memory;
    uint64_t m_capacity;

private:
    cell_t m_sbo_buffer[SBO_CAPACITY];
    bool m_sbo_active;

    // heap memory allocated directly on m_memory
};

#endif // INFINITERMEMORY_H
