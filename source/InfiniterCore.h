#ifndef INFINITERCORE_H
#define INFINITERCORE_H

#include <cstdint>

#define STACK_CAPACITY 2

#define SET_BIT_1(var, bit)     ( (var) |=  (bit) )
#define SET_BIT_0(var, bit)     ( (var) &= ~(bit) )
#define SWITCH_BIT(var, bit)    ( (var) ^=  (bit) )
#define GET_BIT(var, bit)       ( (var) &   (bit) )

typedef uint64_t cell;

class InfiniterCore
{
public:
    InfiniterCore();

    void clear();
    void reset();

    cell *getData();
    const cell *getData() const;
    uint64_t getSize() const;
    uint64_t getCapacity() const;

    uint8_t getSign() const;

    void reserve(uint64_t size);
    void optimize();
    void optimize_memory();

    void dbgPrint(); // temporary


private:


private:
    static constexpr uint8_t SIGN_BIT   = 1 << 0; // 00000001 (1)
    static constexpr uint8_t SBO_BIT    = 1 << 1; // 00000010 (2) // small buffer optimization
    uint8_t m_bit_state;

    uint64_t m_size;
    
    union{
        cell stack[STACK_CAPACITY];

        struct{
            cell *memory;
            uint64_t capacity;
        } heap;
    } m_data;


};

#endif // INFINITERCORE_H
