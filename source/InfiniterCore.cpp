#include "InfiniterCore.h"

#include <cstdio> // temporary - for dbgprint
#include <algorithm>

InfiniterCore::InfiniterCore()
{
    clear();
}

void InfiniterCore::clear()
{

    m_size = STACK_CAPACITY;
    m_bit_state = 0;

    SET_BIT( m_bit_state, SIGN_BIT );
    SET_BIT( m_bit_state, SBO_BIT );


    #pragma GCC unroll STACK_CAPACITY
    for(int i=0; i<STACK_CAPACITY; i++)
    {
        m_data.stack[i] = 0;
    }
}

void InfiniterCore::reset()
{
    if( !GET_BIT(m_bit_state, SBO_BIT) && m_data.heap.memory != nullptr )
    {
        delete[] m_data.heap.memory;
        m_data.heap.memory = nullptr;
    }

    clear();
}

cell *InfiniterCore::getData()
{
    return GET_BIT(m_bit_state, SBO_BIT) ? m_data.stack : m_data.heap.memory;
}

const cell *InfiniterCore::getData() const
{
    return GET_BIT(m_bit_state, SBO_BIT) ? m_data.stack : m_data.heap.memory;
}

uint64_t InfiniterCore::getSize() const
{
    return m_size;
}

uint64_t InfiniterCore::getCapacity() const
{
    return GET_BIT(m_bit_state, SBO_BIT) ? STACK_CAPACITY : m_data.heap.capacity;
}

uint8_t InfiniterCore::getSign() const
{
    return GET_BIT( m_bit_state, SIGN_BIT );
}

void InfiniterCore::reserve(uint64_t new_size)
{
    // keep highest value as technical buffer (for loops)
    if(new_size == ~UINT64_C(0))
        new_size = ~UINT64_C(0)-1;

    if(GET_BIT(m_bit_state, SBO_BIT))
    {
        if(new_size <= STACK_CAPACITY) return; // handle m_size

        // allocate heap (to tmp variable because of union)
        cell* new_heap = new cell[new_size];

        // move existing data
        std::copy(m_data.stack, m_data.stack + m_size, new_heap);
        // fill new cells with mask000
        std::fill(new_heap + m_size, new_heap + new_size, 0);

        // add new heap to member variables
        m_data.heap.memory = new_heap;
        m_size = new_size;
        CLEAR_BIT( m_bit_state, SBO_BIT );
    }
    else
    {
        if(new_size <= m_data.heap.capacity) return;

        // allocate new heap
        cell* new_heap = new cell[new_size];

        // move existing data
        std::copy(m_data.heap.memory, m_data.heap.memory + m_size, new_heap);
        // fill new cells with mask000
        std::fill(new_heap + m_size, new_heap + new_size, 0);

        // delete old heap and add new heap to member variables
        delete[] m_data.heap.memory;
        m_data.heap.memory = new_heap;
        m_size = new_size;
    }
}

void InfiniterCore::dbgPrint() // temporary
{
    if(GET_BIT(m_bit_state, SBO_BIT))
    {
        printf("SBO ");
        for(int i=0; i<m_size; i++)
        {
            auto value = m_data.stack[m_size -i -1];
            for (int i = 63; i >= 0; --i)
            {
                int bit = (int)((value >> i) & 1);
                if(bit)
                    printf("1");
                else
                    printf("_");
                // printf("%d", (int)((value >> i) & 1));
            }
            printf(" ");
        }
    }
    else
    {
        printf("    ");
        for(int i=0; i<m_size; i++)
        {
            auto value = m_data.heap.memory[m_size -i -1];
            for (int i = 63; i >= 0; --i)
            {
                int bit = (int)((value >> i) & 1);
                if(bit)
                    printf("1");
                else
                    printf("_");
                // printf("%d", (int)((value >> i) & 1));
            }
            printf(" ");
        }
    }
}
