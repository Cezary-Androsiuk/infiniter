#include "InfiniterMemory.hpp"

#include "InfiniterShared.hpp"

#include <new> // std::bad_alloc
#include <algorithm> // std::copy_n, std::fill_n, std::min

/// _im_dbgprintf("--- DEBUG IM %p | Constructed   DEFAULT\n", this);
/// _im_dbgprintf("--- DEBUG IM %p | Constructed   PARAMETER\n", this);
/// _im_dbgprintf("--- DEBUG IM %p | Constructed   COPY\n", this);
/// _im_dbgprintf("--- DEBUG IM %p | Constructed   MOVE\n", this);
/// _im_dbgprintf("--- DEBUG IM %p | Assigned      COPY\n", this);
/// _im_dbgprintf("--- DEBUG IM %p | Assigned      MOVE\n", this);
/// _im_dbgprintf("--- DEBUG IM %p | Delete\n", this);
/// _im_dbgprintf("--- DEBUG IM %p | Other         (...)\n", this);
#if IM_DEBUG_EXECUTION_PRINT
#include <cstdio>
#define _im_dbgprintf(...) printf(__VA_ARGS__);
#else // IM_DEBUG_EXECUTION_PRINT
#define _im_dbgprintf(...)
#endif // IM_DEBUG_EXECUTION_PRINT


/// Macro for limitting capacity of variable
#define IM_CAP_LIMIT(capacity) if(UNLIKELY((capacity) == IM_CAPACITY_LOOP_STOP)) (capacity) = IM_MAX_CAPACITY;

InfiniterMemory::InfiniterMemory() noexcept
    : m_data( m_sbo_buffer )
    , m_capacity( SBO_CAPACITY )
    , m_bits({
        .sbo_active = true
      })
#if IM_CLEAR_ALLOCATED_MEMORY
    , m_sbo_buffer()
#endif // IM_CLEAR_ALLOCATED_MEMORY
{
    _im_dbgprintf("--- DEBUG IM %p | Constructed   DEFAULT\n", this);
}

InfiniterMemory::InfiniterMemory(isize_t p_capacity)
{
    /// NOTE: p_capacity smaller than SBO_CAPACITY will be treated as SBO_CAPACITY

    _im_dbgprintf("--- DEBUG IM %p | Constructed   PARAMETER isize_t\n", this);

    /// often when this constructor will be called, user wants more that SBO size
    if(LIKELY( p_capacity > SBO_CAPACITY ))
    {
        IM_CAP_LIMIT(p_capacity);

        /// heap memory
#if IM_CLEAR_ALLOCATED_MEMORY
        m_data = new icell_t[p_capacity]();
#else // IM_CLEAR_ALLOCATED_MEMORY
        m_data = new icell_t[p_capacity];
#endif // IM_CLEAR_ALLOCATED_MEMORY
        if(m_data == nullptr) throw std::bad_alloc();

        m_capacity = p_capacity;
        m_bits.sbo_active = false;
    }
    else
    {
        /// stack memory // much slower than default constructor
        m_data = m_sbo_buffer;
        m_capacity = SBO_CAPACITY;
        m_bits.sbo_active = true;

#if IM_CLEAR_ALLOCATED_MEMORY
        std::fill_n(m_sbo_buffer, SBO_CAPACITY, ICELL_C(0));
#endif // IM_CLEAR_ALLOCATED_MEMORY
    }
}

InfiniterMemory::InfiniterMemory(const InfiniterMemory &p_source)
{
    _im_dbgprintf("--- DEBUG IM %p | Constructed   COPY\n", this);

    /// set stack or heap
    m_data = p_source.m_bits.sbo_active ? m_sbo_buffer : new icell_t[p_source.m_capacity];
    /// do not clear new allocated memory - it will be overwrited in a while
    /// if operator new thows here std::bad_alloc(), object instance won't be corrupted here

    m_bits.sbo_active = p_source.m_bits.sbo_active;
    m_capacity = p_source.m_capacity;

    /// copy old data
    std::copy_n(p_source.m_data, m_capacity, m_data);
}

InfiniterMemory::InfiniterMemory(InfiniterMemory &&p_source) noexcept
    : m_bits({ .sbo_active = p_source.m_bits.sbo_active })
    , m_capacity( p_source.m_capacity )
{
    _im_dbgprintf("--- DEBUG IM %p | Constructed   MOVE\n", this);

    /// ensure speed up for moving heap memory, stack here is already slow
    if(UNLIKELY( m_bits.sbo_active ))
    {
        m_data = m_sbo_buffer;

        /// copy stack memory
        std::copy_n(p_source.m_data, m_capacity, m_data);
    }
    else
    {
        /// move already allocated memory from source here
        m_data = p_source.m_data;
        p_source.m_data = nullptr;
    }

#if IM_ENSURE_NEW_OBJECT_AFTER_MOVE
    /// reset source by hand (to prevent memory deallocation)
    p_source.m_data = p_source.m_sbo_buffer;
    p_source.m_capacity = SBO_CAPACITY;
    p_source.m_bits.sbo_active = true;

#if IM_CLEAR_ALLOCATED_MEMORY
    std::fill_n(p_source.m_sbo_buffer, SBO_CAPACITY, ICELL_C(0));
#endif // IM_CLEAR_ALLOCATED_MEMORY
#endif // IM_ENSURE_NEW_OBJECT_AFTER_MOVE
}

InfiniterMemory::~InfiniterMemory() noexcept
{
    _im_dbgprintf("--- DEBUG IM %p | Delete\n", this);

    if(!m_bits.sbo_active)
    {
        delete [] m_data;
    }
}

void InfiniterMemory::reset() noexcept
{
    _im_dbgprintf("--- DEBUG IM %p | \n", this);

    if(!m_bits.sbo_active)
    {
        delete [] m_data; /// for scalar array exception won't be thrown
    }

    m_data = m_sbo_buffer;
    m_capacity = SBO_CAPACITY;
    m_bits.sbo_active = true;

    std::fill_n(m_sbo_buffer, SBO_CAPACITY, ICELL_C(0));
}

void InfiniterMemory::reserve(isize_t p_new_capacity)
{
    _im_dbgprintf("--- DEBUG IM %p | Other         reserve isize_t\n", this);

    /// who wants to reserve less than he has?
    /// this condition also covers when p_new_capacity < SBO_CAPACITY
    /// m_capacity smallest value is SBO_CAPACITY,
    /// and it happends only when SBO is active
    if(UNLIKELY( p_new_capacity <= m_capacity ))
        return;


    IM_CAP_LIMIT(p_new_capacity);

    /// m_capacity < p_new_capacity at this point

    icell_t *tmp_data = new icell_t[p_new_capacity];
    /// will throw if allocation failed, but object instance, won't be corrupted

    /// copy old data
    std::copy_n(m_data, m_capacity, tmp_data);

    /// set new cells to 0
    isize_t cells_added = p_new_capacity - m_capacity;
    std::fill_n(tmp_data + m_capacity, cells_added, ICELL_C(0));

    /// dealocate old memory if allocated
    if(!m_bits.sbo_active)
    {
        delete [] m_data;
    }

    m_data = tmp_data;
    m_bits.sbo_active = false;
    m_capacity = p_new_capacity;

}

void InfiniterMemory::reserve(const InfiniterMemory &p_source)
{
    _im_dbgprintf("--- DEBUG IM %p | Other         reserve InfiniterMemory\n", this);

    this->reserve(p_source.m_capacity);
}

void InfiniterMemory::extend(isize_t p_additional_capacity)
{
    _im_dbgprintf("--- DEBUG IM %p | Other         extend isize_t\n", this);

    this->reserve(m_capacity + p_additional_capacity);
}

// void InfiniterMemory::shrink()
// {
//     /// auto shinks to first non zero cell
//     /// for example:
//     /// INPUT:  000 000 000 100 000 000 101 101 000
//     /// OUTPUT:             100 000 000 101 101 000

//     /// might require m_size update in InfiniterCore

//     /// SBO is smallest possible size, so if active then skip it
//     /// propably not often someone want to shrink SBO more
//     if(UNLIKELY(m_bits.sbo_active))
//         return;

//     /// find first non zero cell going from the left (from MSB)
//     /// if msb_index is smaller than SBO_CAPACITY just enable SBO
//     isize_t msb_index = m_capacity;
//     while( !(m_data[--msb_index]) && msb_index>=SBO_CAPACITY )
//         ;


//     if(msb_index+1 == SBO_CAPACITY)
//     {
//         /// shrink to match SBO

//         /// copy old data
//         std::copy_n(m_data, SBO_CAPACITY, m_sbo_buffer);

//         /// dealocate old memory
//         delete [] m_data;

//         m_data = m_sbo_buffer;
//         m_bits.sbo_active = true;
//         m_capacity = SBO_CAPACITY;
//     }
//     else
//     {
//         /// shrink heap

//         isize_t new_capacity = msb_index +1;

//         icell_t *tmp_data = new icell_t[new_capacity];
//         /// will throw if allocation failed, but object instance, won't be corrupted

//         /// copy old data
//         std::copy_n(m_data, new_capacity, tmp_data);

//         /// dealocate old memory
//         delete [] m_data;

//         m_data = tmp_data;
//         m_bits.sbo_active = false;
//         m_capacity = new_capacity;
//     }

// }

void InfiniterMemory::shrink(isize_t p_target_capacity)
{
    _im_dbgprintf("--- DEBUG IM %p | Other         shrink isize_t\n", this);

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
    if(UNLIKELY(m_bits.sbo_active))
        return;

    /// shrinking to larger capacity? must be nice
    if(UNLIKELY( p_target_capacity >= m_capacity ))
        return;

    if(p_target_capacity <= SBO_CAPACITY)
    {
        /// shrink to match SBO

        /// copy old data
        std::copy_n(m_data, SBO_CAPACITY, m_sbo_buffer);

        /// dealocate old memory
        delete [] m_data;

        m_data = m_sbo_buffer;
        m_bits.sbo_active = true;
        m_capacity = SBO_CAPACITY;
    }
    else
    {
        /// shrink heap

        icell_t *tmp_data = new icell_t[p_target_capacity];
        /// will throw if allocation failed, but object instance, won't be corrupted

        /// copy old data
        std::copy_n(m_data, p_target_capacity, tmp_data);

        /// dealocate old memory
        delete [] m_data;

        m_data = tmp_data;
        m_bits.sbo_active = false;
        m_capacity = p_target_capacity;
    }
}

void InfiniterMemory::assign(const InfiniterMemory &p_source)
{
    if( &p_source != this )
    {
        icell_t *tmp_data = p_source.m_bits.sbo_active ? m_sbo_buffer : new icell_t[p_source.m_capacity];
        /// will throw if allocation failed, but object instance, won't be corrupted

        /// dealocate old memory if allocated
        if(!m_bits.sbo_active)
        {
            delete [] m_data;
        }

        m_data = tmp_data;
        m_bits.sbo_active = p_source.m_bits.sbo_active;
        m_capacity = p_source.m_capacity;

        /// copy old data
        std::copy_n(p_source.m_data, m_capacity, m_data);
    }
}

void InfiniterMemory::assign(InfiniterMemory &&p_source)
{
    if( &p_source != this )
    {
        m_bits.sbo_active = p_source.m_bits.sbo_active;
        m_capacity = p_source.m_capacity;

        /// ensure speed up for moving heap memory, stack here is already slow
        if(UNLIKELY( m_bits.sbo_active ))
        {
            m_data = m_sbo_buffer;

            /// copy stack memory
            std::copy_n(p_source.m_data, m_capacity, m_data);
        }
        else
        {
            /// move already allocated memory from source here
            m_data = p_source.m_data;
            p_source.m_data = nullptr;
        }

#if IM_ENSURE_NEW_OBJECT_AFTER_MOVE
        /// reset source by hand (to prevent memory deallocation)
        p_source.m_data = p_source.m_sbo_buffer;
        p_source.m_capacity = SBO_CAPACITY;
        p_source.m_bits.sbo_active = true;

#if IM_CLEAR_ALLOCATED_MEMORY
        std::fill_n(p_source.m_sbo_buffer, SBO_CAPACITY, ICELL_C(0));
#endif // IM_CLEAR_ALLOCATED_MEMORY
#endif // IM_ENSURE_NEW_OBJECT_AFTER_MOVE
    }
}

#if IM_ENABLE_DB_PRINT_METHOD
#include <cstdio>
void InfiniterMemory::dbg_print() const
{
    printf("IM obj: %p, capacity: %llu, sbo: %c\n", this, m_capacity, m_bits.sbo_active + '0');
    for(isize_t i=0; i<m_capacity; i++)
    {
        printf("%016llx ", m_data[m_capacity - ISIZE_C(1) - i]);
    }
    printf("\n");
}
#endif // IM_ENABLE_DB_PRINT_METHOD

bool InfiniterMemory::isSBOActive() const
{
    return m_bits.sbo_active;
}

InfiniterMemory &InfiniterMemory::operator =(const InfiniterMemory &p_source)
{
    _im_dbgprintf("--- DEBUG IM %p | Assigned      COPY\n", this);

    if( &p_source != this )
    {
        icell_t *tmp_data = p_source.m_bits.sbo_active ? m_sbo_buffer : new icell_t[p_source.m_capacity];
        /// will throw if allocation failed, but object instance, won't be corrupted

        /// dealocate old memory if allocated
        if(!m_bits.sbo_active)
        {
            delete [] m_data;
        }

        m_data = tmp_data;
        m_bits.sbo_active = p_source.m_bits.sbo_active;
        m_capacity = p_source.m_capacity;

        /// copy old data
        std::copy_n(p_source.m_data, m_capacity, m_data);
    }

    return *this;
}

InfiniterMemory &InfiniterMemory::operator =(InfiniterMemory &&p_source)
{
    _im_dbgprintf("--- DEBUG IM %p | Assigned      MOVE\n", this);

    if( &p_source != this )
    {
        m_bits.sbo_active = p_source.m_bits.sbo_active;
        m_capacity = p_source.m_capacity;

        /// ensure speed up for moving heap memory, stack here is already slow
        if(UNLIKELY( m_bits.sbo_active ))
        {
            m_data = m_sbo_buffer;

            /// copy stack memory
            std::copy_n(p_source.m_data, m_capacity, m_data);
        }
        else
        {
            /// move already allocated memory from source here
            m_data = p_source.m_data;
            p_source.m_data = nullptr;
        }

#if IM_ENSURE_NEW_OBJECT_AFTER_MOVE
        /// reset source by hand (to prevent memory deallocation)
        p_source.m_data = p_source.m_sbo_buffer;
        p_source.m_capacity = SBO_CAPACITY;
        p_source.m_bits.sbo_active = true;

#if IM_CLEAR_ALLOCATED_MEMORY
        std::fill_n(p_source.m_sbo_buffer, SBO_CAPACITY, ICELL_C(0));
#endif // IM_CLEAR_ALLOCATED_MEMORY
#endif // IM_ENSURE_NEW_OBJECT_AFTER_MOVE
    }

    return *this;
}
