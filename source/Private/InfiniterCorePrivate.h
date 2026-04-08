#ifndef INFINITERCOREPRIVATE_H
#define INFINITERCOREPRIVATE_H

#define STACK_CAPACITY 2

typedef unsigned long long cell;
typedef unsigned long long uint_64;

class InfiniterCorePrivate
{
public:
    InfiniterCorePrivate();

    void clear();
    void reset();

    cell *getData();
    const cell *getData() const;
    const uint_64 &getSize() const;
    const uint_64 &getCapacity() const;

    void reserve(uint_64 size);
    void optimize();
    void optimize_memory();

    void dbgPrint(); // temporary


private:


private:
    bool m_sbo_active; // small buffer optimization

    uint_64 m_size;
    
    union{
        cell stack[STACK_CAPACITY];

        struct{
            cell *memory;
            uint_64 capacity;
        } heap;
    } m_data;
};

#endif // INFINITERCOREPRIVATE_H
