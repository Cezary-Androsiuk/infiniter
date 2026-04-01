#include "InfiniterCorePrivate.h"

#include <cstdio>

InfiniterCorePrivate::InfiniterCorePrivate()
{
    clear();
}

void InfiniterCorePrivate::clear()
{
    m_length = 0;
    m_sboActive = true;

#pragma GCC unroll STACK_CELLS_SIZE
    for(int i=0; i<STACK_CELLS_SIZE; i++)
    {
        m_data.stack[i] = 0;
    }
}

cell *InfiniterCorePrivate::getData()
{
    return m_sboActive ? m_data.stack : m_data.heap.memory;
}

const cell *InfiniterCorePrivate::getData() const
{
    return m_sboActive ? m_data.stack : m_data.heap.memory;
}

void InfiniterCorePrivate::dbgPrint()
{
    if(m_sboActive)
    {
        for(int i=0; i<m_length; i++)
        {
            printf("%llu ", m_data.stack[m_length -i -1]);
        }
    }
    else
    {
        for(int i=0; i<m_length; i++)
        {
            printf("%llu ", m_data.heap.memory[m_length -i -1]);
        }
    }

}
