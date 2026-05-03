#include "InfiniterMemory.h"

#include <new> // std::bad_alloc
#include <algorithm> // std::copy_n, std::fill_n
// #include <cstring> // std::memcpy
#include <cstdio> // tmp

#include "InfiniterCommon.hpp"

InfiniterMemory::InfiniterMemory() noexcept
    : m_memory( m_sbo_buffer )
    , m_capacity( SBO_CAPACITY )
    , m_sbo_active( true )
#if CLEAR_ALLOCATED_MEMORY
    , m_sbo_buffer()
#endif
{
    printf("default C\n");
}

InfiniterMemory::InfiniterMemory(uint64_t p_capacity)
{
    printf("parameter C\n");
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
        // std::memset(m_sbo_buffer, 0, SBO_CAPACITY);
        std::fill_n(m_sbo_buffer, SBO_CAPACITY, 0);
#endif
    }
}

InfiniterMemory::InfiniterMemory(const InfiniterMemory &p_source)
    : m_sbo_active( p_source.m_sbo_active )
    , m_capacity( p_source.m_capacity )
{
    printf("copy C\n");
    /// set stack or heap
    m_memory = m_sbo_active ? m_sbo_buffer : new cell_t[m_capacity];
    /// if operator new thows here std::bad_alloc() object will be hardly corrupted
    /// because capacity and sbo flag are already set

    /// copy old data
    // std::memcpy(m_memory, p_source.m_memory, m_capacity);
    std::copy_n(p_source.m_memory, m_capacity, m_memory);
}

InfiniterMemory::InfiniterMemory(InfiniterMemory &&p_source)
    : m_sbo_active( p_source.m_sbo_active )
    , m_capacity( p_source.m_capacity )
{
    printf("move C\n");
    if(UNLIKELY( m_sbo_active )) /// ensure speed up moving heap memory, stack here is already slow
    {
        m_memory = m_sbo_buffer;

        /// copy stack memory
        // std::memcpy(m_memory, p_source.m_memory, m_capacity);
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
    // std::memset(p_source.m_sbo_buffer, 0, SBO_CAPACITY);
    std::fill_n(p_source.m_sbo_buffer, SBO_CAPACITY, 0);
#endif
#endif
}

InfiniterMemory::~InfiniterMemory()
{
    if(!m_sbo_active)
    {
        delete [] m_memory;
    }
}

void InfiniterMemory::reset()
{
    if(!m_sbo_active)
    {
        delete [] m_memory;
    }

    m_memory = m_sbo_buffer;
    m_capacity = SBO_CAPACITY;
    m_sbo_active = true;

#if CLEAR_ALLOCATED_MEMORY
    // std::memset(m_sbo_buffer, 0, SBO_CAPACITY);
    std::fill_n(m_sbo_buffer, SBO_CAPACITY, 0);
#endif
}

void InfiniterMemory::print() const
{
    for(int i=0; i<m_capacity; i++)
    {
        // for(int j=0; j<64; j++)
        // {

        // }
        printf("%016llx ", m_memory[m_capacity-1-i]);
    }
    printf("\n");
}
