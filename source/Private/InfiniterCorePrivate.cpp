#include "InfiniterCorePrivate.h"

#include <cstdio> // temporary - for dbgprint 
#include <algorithm>

InfiniterCorePrivate::InfiniterCorePrivate()
{
    clear();
}

void InfiniterCorePrivate::clear()
{
    m_size = STACK_CAPACITY;
    m_sbo_active = true;

    #pragma GCC unroll STACK_CAPACITY
    for(int i=0; i<STACK_CAPACITY; i++)
    {
        m_data.stack[i] = 0;
    }
}

void InfiniterCorePrivate::reset()
{
    if( !m_sbo_active && m_data.heap.memory != 0 )
    {
        delete[] m_data.heap.memory;
    }

    clear();
}

cell *InfiniterCorePrivate::getData()
{
    return m_sbo_active ? m_data.stack : m_data.heap.memory;
}

const cell *InfiniterCorePrivate::getData() const
{
    return m_sbo_active ? m_data.stack : m_data.heap.memory;
}

const uint_64 &getSize() const
{
    return m_size;
}

const uint_64 &getCapacity() const
{
    return m_sbo_active ? STACK_CAPACITY : m_data.heap.capacity;
}

void InfiniterCorePrivate::reserve(uint_64 new_size)
{
    if(m_sbo_active)
    {
        if(new_size <= STACK_CAPACITY) return; // handle m_size

        // allocate heap (to tmp variable because of union)
        cell* new_heap = new cell[new_size];

        // move existing data
        std::copy(m_data.stack, m_data.stack + m_size, new_heap)

        // add new heap to member variables
        m_data.heap.memory = new_heap;
        m_size = new_size;
    }
    else
    {
        if(new_size <= m_data.heap.capacity) return;

        // allocate new heap
        cell* new_heap = new cell[new_size];

        // move existing data
        std::copy(m_data.heap.memory, m_data.heap.memory + m_size, new_heap)

        // delete old heap and add new heap to member variables
        delete[] m_data.heap.memory;
        m_data.heap.memory = new_heap;
        m_size = new_size;
    }
}

void InfiniterCorePrivate::dbgPrint() // temporary
{
    if(m_sbo_active)
    {
        for(int i=0; i<m_size; i++)
        {
            printf("%llu ", m_data.stack[m_size -i -1]);
        }
    }
    else
    {
        for(int i=0; i<m_size; i++)
        {
            printf("%llu ", m_data.heap.memory[m_size -i -1]);
        }
    }
}
