#ifndef INFINITERMEMORY_H
#define INFINITERMEMORY_H

#include <cstdint>

#include "InfiniterCommon.hpp"

class InfiniterMemory
{
protected: public:
    /// Constructor
    InfiniterMemory() noexcept;
    InfiniterMemory(uint64_t p_capacity); /// throws bad_alloc

    /// Copy Constructor
    InfiniterMemory(const InfiniterMemory &p_source);
    InfiniterMemory& operator =(const InfiniterMemory &) = delete;

    /// Move Constructor
    InfiniterMemory(InfiniterMemory &&p_source);
    InfiniterMemory& operator =(InfiniterMemory &&) = delete;

    ~InfiniterMemory();

    void reset();

    void print() const;

protected:
    cell_t *m_memory;
    uint64_t m_capacity;

private:
    cell_t m_sbo_buffer[SBO_CAPACITY];
    bool m_sbo_active;

    // heap memory allocated directly on m_memory
};

#endif // INFINITERMEMORY_H
