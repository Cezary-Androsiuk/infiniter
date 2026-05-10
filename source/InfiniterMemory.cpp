#include "InfiniterMemory.h"

#include <new> // std::bad_alloc
#include <algorithm> // std::copy_n, std::fill_n
// #include <cstring> // std::memcpy
#include <cstdio> // tmp

#include "InfiniterCommon.hpp"



/// Constructed     TYPE    %p
/// Assigned        TYPE    %p
/// DEL                     %p
/// im_dbgprintf("Constructed   DEFAULT     %p\n", this);
/// im_dbgprintf("Constructed   PARAMETER   %p\n", this);
/// im_dbgprintf("Constructed   COPY        %p\n", this);
/// im_dbgprintf("Constructed   MOVE        %p\n", this);
/// im_dbgprintf("DEL                       %p\n", this);
/// im_dbgprintf("Assigned      COPY        %p\n", this);
/// im_dbgprintf("Assigned      MOVE        %p\n", this);
#if INFINITER_MEMORY_DEBUG_PRINT
#define im_dbgprintf(...) printf(__VA_ARGS__);
#else
#define im_dbgprintf(...)
#endif


InfiniterMemory::InfiniterMemory() noexcept
    : m_memory( m_sbo_buffer )
    , m_capacity( SBO_CAPACITY )
    , m_sbo_active( true )
#if CLEAR_ALLOCATED_MEMORY
    , m_sbo_buffer()
#endif
{
    im_dbgprintf("Constructed   DEFAULT     %p\n", this);
}

InfiniterMemory::InfiniterMemory(uint64_t p_capacity)
{
    im_dbgprintf("Constructed   PARAMETER   %p\n", this);

    /// often when this constructor will be called, user wants more that SBO size
    if(LIKELY( p_capacity > SBO_CAPACITY ))
    {
        /// heap memory
#if CLEAR_ALLOCATED_MEMORY
        m_memory = new cell_t[p_capacity]();
#else
        m_memory = new cell_t[p_capacity];
#endif
        if(m_memory == nullptr) throw std::bad_alloc();

        m_capacity = p_capacity;
        m_sbo_active = false;
    }
    else
    {
        /// stack memory // much slower than default constructor
        m_memory = m_sbo_buffer;
        m_capacity = SBO_CAPACITY;
        m_sbo_active = true;

#if CLEAR_ALLOCATED_MEMORY
        std::fill_n(m_sbo_buffer, SBO_CAPACITY, 0);
#endif
    }
}

InfiniterMemory::InfiniterMemory(const InfiniterMemory &p_source)
{
#if INFINITER_MEMORY_DEBUG_PRINT
    im_dbgprintf("Constructed   COPY        %p\n", this);
#endif

    /// set stack or heap
    m_memory = p_source.m_sbo_active ? m_sbo_buffer : new cell_t[p_source.m_capacity];
    /// do not clear new allocated memory - it will be overwrited in a while
    /// if operator new thows here std::bad_alloc(), object instance won't be corrupted here

    m_sbo_active = p_source.m_sbo_active;
    m_capacity = p_source.m_capacity;

    /// copy old data
    std::copy_n(p_source.m_memory, m_capacity, m_memory);
}

InfiniterMemory::InfiniterMemory(InfiniterMemory &&p_source) noexcept
    : m_sbo_active( p_source.m_sbo_active )
    , m_capacity( p_source.m_capacity )
{
    im_dbgprintf("Constructed   MOVE        %p\n", this);

    /// ensure speed up for moving heap memory, stack here is already slow
    if(UNLIKELY( m_sbo_active ))
    {
        m_memory = m_sbo_buffer;

        /// copy stack memory
        std::copy_n(p_source.m_memory, m_capacity, m_memory);
    }
    else
    {
        /// move already allocated memory from source here
        m_memory = p_source.m_memory;
        p_source.m_memory = nullptr;
    }

#if ENSURE_NEW_OBJECT_AFTER_MOVE
    /// reset source by hand (to prevent memory deallocation)
    p_source.m_memory = p_source.m_sbo_buffer;
    p_source.m_capacity = SBO_CAPACITY;
    p_source.m_sbo_active = true;

#if CLEAR_ALLOCATED_MEMORY
    std::fill_n(p_source.m_sbo_buffer, SBO_CAPACITY, 0);
#endif
#endif
}

InfiniterMemory::~InfiniterMemory()
{
    im_dbgprintf("DEL                       %p\n", this);

    if(!m_sbo_active)
    {
        delete [] m_memory;
    }
}

void InfiniterMemory::reserve(uint64_t p_new_capacity)
{
    /// who wants to reserve less than he has?
    /// this condition also covers when p_new_capacity < SBO_CAPACITY
    /// m_capacity smallest value is SBO_CAPACITY,
    /// and it happends only when SBO is active
    if(UNLIKELY( p_new_capacity <= m_capacity ))
        return;

    /// m_capacity < p_new_capacity at this point

    cell_t *tmp_memory = new cell_t[p_new_capacity];
    /// will throw if allocation failed, but object instance, won't be corrupted

    /// copy old data
    std::copy_n(m_memory, m_capacity, tmp_memory);

    /// set new cells to 0
    uint64_t cells_added = p_new_capacity - m_capacity;
    std::fill_n(tmp_memory + m_capacity, cells_added, 0);

    /// dealocate old memory if allocated
    if(!m_sbo_active)
    {
        delete [] m_memory;
    }

    m_memory = tmp_memory;
    m_sbo_active = false;
    m_capacity = p_new_capacity;

}

void InfiniterMemory::reserve(const InfiniterMemory &p_source)
{
    this->reserve(p_source.m_capacity);
}

void InfiniterMemory::extend(uint64_t p_additional_capacity)
{
    this->reserve(m_capacity + p_additional_capacity);
}

void InfiniterMemory::shrink()
{
    /// auto shinks to first non zero cell
    /// for example:
    /// INPUT:  000 000 000 100 000 000 101 101 000
    /// OUTPUT:             100 000 000 101 101 000

    /// might require m_size update in InfiniterCore

    /// SBO is smallest possible size, so if active then skip it
    /// propably not often someone want to shrink SBO more
    if(UNLIKELY(m_sbo_active))
        return;

    /// find first non zero cell going from the left (from MSB)
    /// if msb_index is smaller than SBO_CAPACITY just enable SBO
    uint64_t msb_index = m_capacity;
    while( !(m_memory[--msb_index]) && msb_index>=SBO_CAPACITY )
        ;


    if(msb_index+1 == SBO_CAPACITY)
    {
        /// shrink to match SBO

        /// copy old data
        std::copy_n(m_memory, SBO_CAPACITY, m_sbo_buffer);

        /// dealocate old memory
        delete [] m_memory;

        m_memory = m_sbo_buffer;
        m_sbo_active = true;
        m_capacity = SBO_CAPACITY;
    }
    else
    {
        /// shrink heap

        uint64_t new_capacity = msb_index +1;

        cell_t *tmp_memory = new cell_t[new_capacity];
        /// will throw if allocation failed, but object instance, won't be corrupted

        /// copy old data
        std::copy_n(m_memory, new_capacity, tmp_memory);

        /// dealocate old memory
        delete [] m_memory;

        m_memory = tmp_memory;
        m_sbo_active = false;
        m_capacity = new_capacity;
    }

}

void InfiniterMemory::shrink(uint64_t p_target_capacity)
{
    /// shinks to fit target capacity
    /// for example when target capacity is 4:
    /// INPUT:  000 000 000 100 000 000 101 101 000
    /// OUTPUT:                     000 101 101 000
    /// for example when target capacity is 7:
    /// INPUT:  000 000 000 100 000 000 101 101 000
    /// OUTPUT:         000 100 000 000 101 101 000

    /// should be used only by InfiniterCore after changing m_size

    /// SBO is smallest possible size, so if active then skip it
    /// propably not often someone want to shrink SBO more
    if(UNLIKELY(m_sbo_active))
        return;

    /// shrinking to larger capacity? must be nice
    if(UNLIKELY( p_target_capacity >= m_capacity ))
        return;

    if(p_target_capacity == SBO_CAPACITY)
    {
        /// shrink to match SBO

        /// copy old data
        std::copy_n(m_memory, SBO_CAPACITY, m_sbo_buffer);

        /// dealocate old memory
        delete [] m_memory;

        m_memory = m_sbo_buffer;
        m_sbo_active = true;
        m_capacity = SBO_CAPACITY;
    }
    else
    {
        /// shrink heap

        cell_t *tmp_memory = new cell_t[p_target_capacity];
        /// will throw if allocation failed, but object instance, won't be corrupted

        /// copy old data
        std::copy_n(m_memory, p_target_capacity, tmp_memory);

        /// dealocate old memory
        delete [] m_memory;

        m_memory = tmp_memory;
        m_sbo_active = false;
        m_capacity = p_target_capacity;
    }
}

void InfiniterMemory::reset() noexcept
{
    if(!m_sbo_active)
    {
        delete [] m_memory; /// for scalar array exception won't be thrown
    }

    m_memory = m_sbo_buffer;
    m_capacity = SBO_CAPACITY;
    m_sbo_active = true;

#if CLEAR_ALLOCATED_MEMORY
    std::fill_n(m_sbo_buffer, SBO_CAPACITY, 0);
#endif
}

void InfiniterMemory::dbg_print() const
{
    printf("obj: %p, size: %llu\n", this, m_capacity);
    for(uint64_t i=0; i<m_capacity; i++)
    {
        printf("%016llx ", m_memory[m_capacity-1-i]);
    }
    printf("\n");
}

bool InfiniterMemory::isSBOActive() const
{
    return m_sbo_active;
}

InfiniterMemory &InfiniterMemory::operator =(const InfiniterMemory &p_source)
{
    im_dbgprintf("Assigned      COPY        %p\n", this);

    if( &p_source != this )
    {
        cell_t *tmp_memory = p_source.m_sbo_active ? m_sbo_buffer : new cell_t[p_source.m_capacity];
        /// will throw if allocation failed, but object instance, won't be corrupted

        /// dealocate old memory if allocated
        if(!m_sbo_active)
        {
            delete [] m_memory;
        }

        m_memory = tmp_memory;
        m_sbo_active = p_source.m_sbo_active;
        m_capacity = p_source.m_capacity;

        /// copy old data
        std::copy_n(p_source.m_memory, m_capacity, m_memory);
    }

    return *this;
}

InfiniterMemory &InfiniterMemory::operator =(InfiniterMemory &&p_source)
{
    im_dbgprintf("Assigned      MOVE        %p\n", this);

    if( &p_source != this )
    {
        m_sbo_active = p_source.m_sbo_active;
        m_capacity = p_source.m_capacity;

        /// ensure speed up for moving heap memory, stack here is already slow
        if(UNLIKELY( m_sbo_active ))
        {
            m_memory = m_sbo_buffer;

            /// copy stack memory
            std::copy_n(p_source.m_memory, m_capacity, m_memory);
        }
        else
        {
            /// move already allocated memory from source here
            m_memory = p_source.m_memory;
            p_source.m_memory = nullptr;
        }

#if ENSURE_NEW_OBJECT_AFTER_MOVE
        /// reset source by hand (to prevent memory deallocation)
        p_source.m_memory = p_source.m_sbo_buffer;
        p_source.m_capacity = SBO_CAPACITY;
        p_source.m_sbo_active = true;

#if CLEAR_ALLOCATED_MEMORY
        std::fill_n(p_source.m_sbo_buffer, SBO_CAPACITY, 0);
#endif
#endif
    }

    return *this;
}
