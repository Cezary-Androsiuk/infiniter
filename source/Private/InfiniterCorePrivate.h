#ifndef INFINITERCOREPRIVATE_H
#define INFINITERCOREPRIVATE_H

#define STACK_CELLS_SIZE 2

typedef unsigned long long cell;
typedef unsigned long long size_t;

class InfiniterCorePrivate
{
public:
    InfiniterCorePrivate();

    void clear();

    cell *getData();
    const cell *getData() const;

    void dbgPrint();


private:


private:
    size_t m_length;
    bool m_sboActive;

    union{
        cell stack[STACK_CELLS_SIZE];

        struct{
            cell *memory;
            size_t heapMemReserved;
        } heap;
    } m_data;
};

#endif // INFINITERCOREPRIVATE_H
