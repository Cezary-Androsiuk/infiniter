#pragma once

#include "InfiniterBit.hpp"

#include "InfiniterShared.hpp"
#include "InfiniterException.hpp"

#include <utility> // std::move
#include <stdexcept> // std::out_of_range
#include <string> // std::to_string

/// _ib_dbgprintf("--- DEBUG IB %p | Constructed   DEFAULT\n", this);
/// _ib_dbgprintf("--- DEBUG IB %p | Constructed   PARAMETER\n", this);
/// _ib_dbgprintf("--- DEBUG IB %p | Constructed   COPY\n", this);
/// _ib_dbgprintf("--- DEBUG IB %p | Constructed   MOVE\n", this);
/// _ib_dbgprintf("--- DEBUG IB %p | Assigned      COPY\n", this);
/// _ib_dbgprintf("--- DEBUG IB %p | Assigned      MOVE\n", this);
/// _ib_dbgprintf("--- DEBUG IB %p | Delete\n", this);
/// _ib_dbgprintf("--- DEBUG IB %p | Other         (...)\n", this);
#if IB_DEBUG_EXECUTION_PRINT
#include <cstdio>
#define _ib_dbgprintf(...) printf(__VA_ARGS__);
#else // IB_DEBUG_EXECUTION_PRINT
#define _ib_dbgprintf(...)
#endif // IB_DEBUG_EXECUTION_PRINT

template<typename InfiniterDerived>
InfiniterBit<InfiniterDerived>::InfiniterBit() noexcept
    : InfiniterCore<InfiniterDerived>()
{
    _ib_dbgprintf("--- DEBUG IB %p | Constructed   DEFAULT\n", this);
}

template<typename InfiniterDerived>
InfiniterBit<InfiniterDerived>::InfiniterBit(int64_t p_value)
    : InfiniterCore<InfiniterDerived>(p_value)
{
    _ib_dbgprintf("--- DEBUG IB %p | Constructed   PARAMETER int64_t\n", this);
}

template<typename InfiniterDerived>
InfiniterBit<InfiniterDerived>::InfiniterBit(int64_t p_value, isize_t p_capacity)
    : InfiniterCore<InfiniterDerived>(p_value, p_capacity)
{
    _ib_dbgprintf("--- DEBUG IB %p | Constructed   PARAMETER int64_t isize_t\n", this);
}

template<typename InfiniterDerived>
InfiniterBit<InfiniterDerived>::InfiniterBit(icell_t p_value, isize_t p_capacity, bool p_negative_value)
    : InfiniterCore<InfiniterDerived>(p_value, p_capacity, p_negative_value)
{
    _ib_dbgprintf("--- DEBUG IB %p | Constructed   PARAMETER icell_t isize_t bool\n", this);
}

template<typename InfiniterDerived>
InfiniterBit<InfiniterDerived>::InfiniterBit(const icell_t *p_array, isize_t p_size, bool p_negative_value)
    : InfiniterCore<InfiniterDerived>(p_array, p_size, p_negative_value)
{
    _ib_dbgprintf("--- DEBUG IB %p | Constructed   PARAMETER cell_t* isize_t bool\n", this);
}

template<typename InfiniterDerived>
InfiniterBit<InfiniterDerived>::InfiniterBit(const InfiniterDerived &p_source)
    : InfiniterCore<InfiniterDerived>(p_source)
{
    _ib_dbgprintf("--- DEBUG IB %p | Constructed IC COPY\n", this);
}

template<typename InfiniterDerived>
InfiniterBit<InfiniterDerived>::InfiniterBit(InfiniterDerived &&p_source) noexcept
    : InfiniterCore<InfiniterDerived>(std::move(p_source))
{
    _ib_dbgprintf("--- DEBUG IB %p | Constructed IC MOVE\n", this);
}

template<typename InfiniterDerived>
InfiniterBit<InfiniterDerived>::~InfiniterBit() noexcept
{
    _ib_dbgprintf("--- DEBUG IB %p | Delete\n", this);

    /// everything was done in InfiniterMemory
}

template<typename InfiniterDerived>
bool InfiniterBit<InfiniterDerived>::checkCellPos(isize_t cell_id, uint8_t pos) const
{
    return cell_id < this->getSize() && pos < 64;
}

template<typename InfiniterDerived>
void InfiniterBit<InfiniterDerived>::checkCellPosTry(isize_t cell_id, uint8_t pos) const
{
    if(cell_id >= this->getSize() || pos >= 64)
    {
        if(cell_id >= this->getSize())
        {
            throw InfiniterException::OutOfRange(cell_id, 0, this->getSize()-1);
        }
        else
        {
            throw InfiniterException::OutOfRange(pos, 0, 63);
        }
    }
}

template<typename InfiniterDerived>
icell_t InfiniterBit<InfiniterDerived>::getMSBCell() const noexcept
{
    const icell_t *const data = this->getData();
    const icell_t *cellPtr = data + this->getSize(); /// intentional out of bounds

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return its value
    do{
        if(*(--cellPtr) != ICELL_C(0))
        {
            return *cellPtr;
        }
    }
    while(cellPtr != data);

    /// in any case (0 or non 0 value) return least significant cell value
    return *cellPtr;
}

template<typename InfiniterDerived>
const icell_t *InfiniterBit<InfiniterDerived>::getMSBCellPtr() const noexcept
{
    const icell_t *const data = this->getData();
    const icell_t *cellPtr = data + this->getSize(); /// intentional out of bounds

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return the cell ptr
    do{
        if(*(--cellPtr) != ICELL_C(0))
        {
            return cellPtr;
        }
    }
    while(cellPtr != data);

    /// in any case (0 or non 0 value) return least significant cell
    return cellPtr;
}

template<typename InfiniterDerived>
icell_t *InfiniterBit<InfiniterDerived>::getMSBCellPtr() noexcept
{
    icell_t *const data = this->getData();
    icell_t *cellPtr = data + this->getSize(); /// intentional out of bounds

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return the cell ptr
    do{
        if(*(--cellPtr) != ICELL_C(0))
        {
            return cellPtr;
        }
    }
    while(cellPtr != data);

    /// in any case (0 or non 0 value) return least significant cell
    return cellPtr;
}

template<typename InfiniterDerived>
isize_t InfiniterBit<InfiniterDerived>::getMSBCellPos() const noexcept
{
    const icell_t *const data = this->getData();
    const icell_t *cellPtr = data + this->getSize(); /// intentional out of bounds

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return its pos
    do {
        if(*(--cellPtr) != ICELL_C(0))
        {
            return cellPtr - data;
        }
    }
    while(cellPtr != data);

    /// in any case (0 or non 0 value) return least significant cell pos
    return 0;
}

template<typename InfiniterDerived>
icell_t InfiniterBit<InfiniterDerived>::getMSBCell(uint8_t &r_bit_pos) const noexcept
{
    const icell_t *const data = this->getData();
    const icell_t *cellPtr = data + this->getSize(); /// intentional out of bounds

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return its value
    do{
        if(*(--cellPtr) != ICELL_C(0))
        {
            /// find MSB in cell
            r_bit_pos = ICELL_MSB_POS(*cellPtr);

            return *cellPtr;
        }
    }
    while(cellPtr != data);

    /// in any case (0 or non 0 value) return least significant cell value
    /// but first find MSB in cell
    r_bit_pos = ICELL_MSB_SAFE_POS(*cellPtr);

    return *cellPtr;
}

template<typename InfiniterDerived>
const icell_t *InfiniterBit<InfiniterDerived>::getMSBCellPtr(uint8_t &r_bit_pos) const noexcept
{
    const icell_t *const data = this->getData();
    const icell_t *cellPtr = data + this->getSize(); /// intentional out of bounds

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return its value
    do{
        if(*(--cellPtr) != ICELL_C(0))
        {
            /// find MSB in cell
            r_bit_pos = ICELL_MSB_POS(*cellPtr);

            return cellPtr;
        }
    }
    while(cellPtr != data);

    /// in any case (0 or non 0 value) return least significant cell value
    /// but first find MSB in cell
    r_bit_pos = ICELL_MSB_SAFE_POS(*cellPtr);

    return cellPtr;
}

template<typename InfiniterDerived>
icell_t *InfiniterBit<InfiniterDerived>::getMSBCellPtr(uint8_t &r_bit_pos) noexcept
{
    icell_t *const data = this->getData();
    icell_t *cellPtr = data + this->getSize(); /// intentional out of bounds

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return its value
    do{
        if(*(--cellPtr) != ICELL_C(0))
        {
            /// find MSB in cell
            r_bit_pos = ICELL_MSB_POS(*cellPtr);

            return cellPtr;
        }
    }
    while(cellPtr != data);

    /// in any case (0 or non 0 value) return least significant cell value
    /// but first find MSB in cell
    r_bit_pos = ICELL_MSB_SAFE_POS(*cellPtr);

    return cellPtr;
}

template<typename InfiniterDerived>
isize_t InfiniterBit<InfiniterDerived>::getMSBCellPos(uint8_t &r_bit_pos) const noexcept
{

    const icell_t *const data = this->getData();
    const icell_t *cellPtr = data + this->getSize(); /// intentional out of bounds

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return its pos
    do {
        if(*(--cellPtr) != ICELL_C(0))
        {
            /// find MSB in cell
            r_bit_pos = ICELL_MSB_POS(*cellPtr); /// *cellPtr != 0
            return cellPtr - data;
        }
    }
    while(cellPtr != data);

    /// in any case (0 or non 0 value) return least significant cell pos
    /// find MSB in cell
    r_bit_pos = ICELL_MSB_SAFE_POS(*cellPtr);
    return 0;
}

template<typename InfiniterDerived>
inline const icell_t *InfiniterBit<InfiniterDerived>::getMSBCellPtr(isize_t &r_pos) const noexcept
{

}

template<typename InfiniterDerived>
inline icell_t *InfiniterBit<InfiniterDerived>::getMSBCellPtr(isize_t &r_pos) noexcept
{

}

template<typename InfiniterDerived>
uint8_t InfiniterBit<InfiniterDerived>::getMSBPos() const noexcept
{
    const icell_t *const data = this->getData();
    const icell_t *cellPtr = data + this->getSize(); /// intentional out of bounds

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return its value
    do{
        if(*(--cellPtr) != ICELL_C(0))
        {
            /// find MSB in cell
            return ICELL_MSB_POS(*cellPtr); /// *cellPtr != 0
        }
    }
    while(cellPtr != data);

    /// in any case (0 or non 0 value) return least significant cell value
    /// find MSB in that cell
    return ICELL_MSB_SAFE_POS(*cellPtr);
}

template<typename InfiniterDerived>
isize_t InfiniterBit<InfiniterDerived>::getMSBGlobalPos(isize_t *const overflow) const noexcept
{
    const icell_t *const data = this->getData();
    const icell_t *cellPtr = data + this->getSize(); /// intentional out of bounds

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return its value
    do{
        if(*(--cellPtr) != ICELL_C(0))
        {
            /// find MSB in cell
            isize_t msb_pos = ICELL_MSB_POS(*cellPtr); /// *cellPtr != 0

            /// multiply cell position, cell size and add msb bit position
            /// max value that can occur fit in 70 bits variable (int70_t could be just fine)
            __int128_t result128 = static_cast<__int128_t>(cellPtr - data) * icell_bits + msb_pos;
            isize_t result = static_cast<isize_t>(result128);
            if(overflow)
                *overflow = static_cast<isize_t>(result128 >> isize_bits);
            return result;
        }
    }
    while(cellPtr != data);

    /// in any case (0 or non 0 value) return least significant cell value
    /// find MSB in that cell
    if(overflow) *overflow = ISIZE_C(0);
    return ICELL_MSB_SAFE_POS(*cellPtr);
}

template<typename InfiniterDerived>
InfiniterDerived InfiniterBit<InfiniterDerived>::getMSBGlobalPos() const // throws bad_alloc
{
    const icell_t *const data = this->getData();
    const icell_t *cellPtr = data + this->getSize(); /// intentional out of bounds

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return its value
    do{
        if(*(--cellPtr) != ICELL_C(0))
        {
            /// find MSB in cell
            isize_t msb_pos = ICELL_MSB_POS(*cellPtr); /// *cellPtr != 0

            InfiniterDerived result(2, cellPtr - data); // 2 cells is max that can be emplaced, size require 70bits
            result.multiply(icell_bits).add(msb_pos);
            return result;
        }
    }
    while(cellPtr != data);

    /// in any case (0 or non 0 value) return least significant cell value
    /// find MSB in that cell
    // return std::move(InfiniterDerived(1, ICELL_MSB_SAFE_POS(*cellPtr)));
    return InfiniterDerived(1, ICELL_MSB_SAFE_POS(*cellPtr)); /// copy elision aka RVO (Return Value Optimization)
    /// using move is "pessimising move" and after C++17 copy elision is guaranteed
}









template<typename InfiniterDerived>
icell_t InfiniterBit<InfiniterDerived>::getLSBCell() const noexcept
{
    const icell_t *cellPtr = this->getData();
    const icell_t *const cellEnd = cellPtr + this->getSize() - 1;

    /// iterate from least significant cell, to most significant
    /// until non 0 cell found, then return its value
    while(cellPtr < cellEnd)
    {
        if(*cellPtr != ICELL_C(0))
        {
            return *cellPtr;
        }
        ++cellPtr;
    }

    /// in any case (0 or non 0 value) return most significant cell value
    return *cellPtr;
}

template<typename InfiniterDerived>
const icell_t *InfiniterBit<InfiniterDerived>::getLSBCellPtr() const noexcept
{
    const icell_t *cellPtr = this->getData();
    const icell_t *const cellEnd = cellPtr + this->getSize() - 1;

    /// iterate from least significant cell, to most significant
    /// until non 0 cell found, then return its value
    while(cellPtr < cellEnd)
    {
        if(*cellPtr != ICELL_C(0))
        {
            return cellPtr;
        }
        ++cellPtr;
    }

    /// in any case (0 or non 0 value) return most significant cell value
    return cellPtr;
}

template<typename InfiniterDerived>
icell_t *InfiniterBit<InfiniterDerived>::getLSBCellPtr() noexcept
{
    icell_t *cellPtr = this->getData();
    const icell_t *const cellEnd = cellPtr + this->getSize() - 1;

    /// iterate from least significant cell, to most significant
    /// until non 0 cell found, then return its value
    while(cellPtr < cellEnd)
    {
        if(*cellPtr != ICELL_C(0))
        {
            return cellPtr;
        }
        ++cellPtr;
    }

    /// in any case (0 or non 0 value) return most significant cell value
    return cellPtr;
}

template<typename InfiniterDerived>
isize_t InfiniterBit<InfiniterDerived>::getLSBCellPos() const noexcept
{
    icell_t *cellPtr = this->getData();
    const isize_t size_m1 = this->getSize() -1; /// size_m1 is "size minus 1"
    const icell_t *const cellEnd = cellPtr + size_m1;

    /// iterate from least significant cell, to most significant
    /// until non 0 cell found, then return its pos
    while(cellPtr < cellEnd)
    {
        if(*cellPtr != ICELL_C(0))
        {
            return size_m1 - (cellEnd - cellPtr);
        }
        ++cellPtr;
    }

    /// in any case (0 or non 0 value) return most significant cell pos
    return size_m1;
}

template<typename InfiniterDerived>
icell_t InfiniterBit<InfiniterDerived>::getLSBCell(uint8_t &r_bit_pos) const noexcept
{
    const icell_t *cellPtr = this->getData();
    const icell_t *const cellEnd = cellPtr + this->getSize() - 1;

    /// iterate from least significant cell, to most significant
    /// until non 0 cell found, then return its value
    while(cellPtr < cellEnd)
    {
        if(*cellPtr != ICELL_C(0))
        {
            /// find LSB in that cell
            r_bit_pos = ICELL_LSB_POS(*cellPtr); /// *cellPtr != 0
            return *cellPtr;
        }
        ++cellPtr;
    }

    /// in any case (0 or non 0 value) return most significant cell value
    /// find LSB in that cell
    r_bit_pos = ICELL_LSB_SAFE_POS(*cellEnd); /// *cellPtr != 0
    return *cellPtr;
}

template<typename InfiniterDerived>
const icell_t *InfiniterBit<InfiniterDerived>::getLSBCellPtr(uint8_t &r_bit_pos) const noexcept
{
    const icell_t *cellPtr = this->getData();
    const icell_t *const cellEnd = cellPtr + this->getSize() - 1;

    /// iterate from least significant cell, to most significant
    /// until non 0 cell found, then return its value
    while(cellPtr < cellEnd)
    {
        if(*cellPtr != ICELL_C(0))
        {
            /// find LSB in that cell
            r_bit_pos = ICELL_LSB_POS(*cellPtr); /// *cellPtr != 0
            return cellPtr;
        }
        ++cellPtr;
    }

    /// in any case (0 or non 0 value) return most significant cell value
    /// find LSB in that cell
    r_bit_pos = ICELL_LSB_SAFE_POS(*cellEnd);
    return cellPtr;
}

template<typename InfiniterDerived>
icell_t *InfiniterBit<InfiniterDerived>::getLSBCellPtr(uint8_t &r_bit_pos) noexcept
{
    icell_t *cellPtr = this->getData();
    const icell_t *const cellEnd = cellPtr + this->getSize() - 1;

    /// iterate from least significant cell, to most significant
    /// until non 0 cell found, then return its value
    while(cellPtr < cellEnd)
    {
        if(*cellPtr != ICELL_C(0))
        {
            /// find LSB in that cell
            r_bit_pos = ICELL_LSB_POS(*cellPtr); /// *cellPtr != 0
            return cellPtr;
        }
        ++cellPtr;
    }

    /// in any case (0 or non 0 value) return most significant cell value
    /// find LSB in that cell
    r_bit_pos = ICELL_LSB_SAFE_POS(*cellEnd);
    return cellPtr;
}

template<typename InfiniterDerived>
isize_t InfiniterBit<InfiniterDerived>::getLSBCellPos(uint8_t &r_bit_pos) const noexcept
{
    icell_t *cellPtr = this->getData();
    const isize_t size_m1 = this->getSize() -1; /// size_m1 is "size minus 1"
    const icell_t *const cellEnd = cellPtr + size_m1;

    /// iterate from least significant cell, to most significant
    /// until non 0 cell found, then return its pos
    while(cellPtr < cellEnd)
    {
        if(*cellPtr != ICELL_C(0))
        {
            /// find LSB in that cell
            r_bit_pos = ICELL_LSB_POS(*cellPtr); /// *cellPtr != 0
            return size_m1 - (cellEnd - cellPtr);
        }
        ++cellPtr;
    }

    /// in any case (0 or non 0 value) return most significant cell pos
    /// find LSB in that cell
    r_bit_pos = ICELL_LSB_SAFE_POS(*cellEnd);
    return size_m1;
}

template<typename InfiniterDerived>
inline const icell_t *InfiniterBit<InfiniterDerived>::getLSBCellPtr(isize_t &r_pos) const noexcept
{

}

template<typename InfiniterDerived>
inline icell_t *InfiniterBit<InfiniterDerived>::getLSBCellPtr(isize_t &r_pos) noexcept
{

}

template<typename InfiniterDerived>
uint8_t InfiniterBit<InfiniterDerived>::getLSBPos() const noexcept
{
    icell_t *cellPtr = this->getData();
    const isize_t size_m1 = this->getSize() -1; /// size_m1 is "size minus 1"
    const icell_t *const cellEnd = cellPtr + size_m1;

    /// iterate from least significant cell, to most significant
    /// until non 0 cell found, then return its pos
    while(cellPtr < cellEnd)
    {
        if(*cellPtr != ICELL_C(0))
        {
            /// find LSB in that cell
            return ICELL_LSB_POS(*cellPtr); /// *cellPtr != 0
        }
        ++cellPtr;
    }

    /// in any case (0 or non 0 value) return most significant cell pos
    /// find LSB in that cell
    return ICELL_LSB_SAFE_POS(*cellEnd);
}

template<typename InfiniterDerived>
isize_t InfiniterBit<InfiniterDerived>::getLSBGlobalPos(isize_t *const overflow) const noexcept // handle edge case
{
    const icell_t *const data = this->getData();
    const icell_t *const cellEnd = data + this->getSize();
    const icell_t *cellPtr = data;

    /// iterate from least significant cell, to most significant
    /// until non 0 cell found, then return its value
    while(cellPtr < cellEnd)
    {
        if(*cellPtr != ICELL_C(0))
        {
            /// find LSB in cell
            isize_t lsb_pos = ICELL_LSB_POS(*cellPtr); /// *cellPtr != 0

            /// multiply cell position, cell size and add lsb bit position
            /// max value that can occur fit in 70 bits variable (int70_t could be just fine)
            __int128_t result128 = static_cast<__int128_t>(cellPtr - data) * icell_bits + lsb_pos;
            isize_t result = static_cast<isize_t>(result128);

            if(overflow)
            {
                *overflow = static_cast<isize_t>(result128 >> isize_bits);
            }
            return result;
        }
        ++cellPtr;
    }

    /// in any case (all zeros) return safe LSB of the first cell
    if(overflow) *overflow = ISIZE_C(0);
    return ICELL_LSB_SAFE_POS(*data);
}

template<typename InfiniterDerived>
InfiniterDerived InfiniterBit<InfiniterDerived>::getLSBGlobalPos() const // throws bad_alloc
{
    const icell_t *const data = this->getData();
    const icell_t *const end = data + this->getSize();
    const icell_t *cellPtr = data;

    /// iterate from least significant cell, to most significant
    /// until non 0 cell found, then return its value
    while(cellPtr < end)
    {
        if(*cellPtr != ICELL_C(0))
        {
            /// find LSB in cell
            isize_t lsb_pos = ICELL_LSB_POS(*cellPtr); /// *cellPtr != 0

            InfiniterDerived result(2, cellPtr - data); // 2 cells is max that can be emplaced, size require 70bits

            /// POPRAWKA 1: Mnożymy przez rozmiar komórki (icell_bits), a nie rozmiar wskaźnika/indeksu!
            result.multiply(icell_bits).add(lsb_pos);
            return result;
        }
        ++cellPtr;
    }

    /// in any case (all zeros) return safe LSB of the first cell
    /// POPRAWKA 2: Usunięte std::move
    // return std::move(InfiniterDerived(1, ICELL_LSB_SAFE_POS(*data)));
    return InfiniterDerived(1, ICELL_LSB_SAFE_POS(*data));/// copy elision aka RVO (Return Value Optimization)
    /// using move is "pessimising move" and after C++17 copy elision is guaranteed
}



template<typename InfiniterDerived>
inline InfiniterDerived &InfiniterBit<InfiniterDerived>::invert()
{
    icell_t *data = this->getData();
    icell_t *data_ptr = data + this->getSize();

    while(data != data_ptr--)
    {
        *data_ptr = ~(*data_ptr);
    }

    return this->getRef();
}

template<typename InfiniterDerived>
inline InfiniterDerived InfiniterBit<InfiniterDerived>::invert(const InfiniterDerived &p_value)
{
    return p_value.getCopy().invert();
}




template<typename InfiniterDerived>
ibit_t InfiniterBit<InfiniterDerived>::getBit(isize_t p_cell_index, uint8_t p_pos)
{
    this->checkCellPosTry(p_cell_index, p_pos);
    
    return this->getData()[p_cell_index] & (ICELL_C(1) << p_pos) ? IBIT_TRUE : IBIT_FALSE;
}

template<typename InfiniterDerived>
void InfiniterBit<InfiniterDerived>::setBit(isize_t p_cell_index, uint8_t p_pos)
{
    this->checkCellPosTry(p_cell_index, p_pos);
    
    this->getData()[p_cell_index] |= (ICELL_C(1) << p_pos);
}

template<typename InfiniterDerived>
void InfiniterBit<InfiniterDerived>::clearBit(isize_t p_cell_index, uint8_t p_pos)
{
    this->checkCellPosTry(p_cell_index, p_pos);

    this->getData()[p_cell_index] &= ~(ICELL_C(1) << p_pos);
}

template<typename InfiniterDerived>
void InfiniterBit<InfiniterDerived>::toggleBit(isize_t p_cell_index, uint8_t p_pos)
{
    this->checkCellPosTry(p_cell_index, p_pos);

    this->getData()[p_cell_index] ^= (ICELL_C(1) << p_pos);
}

template<typename InfiniterDerived>
ibit_t InfiniterBit<InfiniterDerived>::getBitUnsafe(uint64_t p_cell_index, uint8_t p_pos) noexcept
{
    return this->getData()[p_cell_index] & (ICELL_C(1) << p_pos) ? IBIT_TRUE : IBIT_FALSE;
}

template<typename InfiniterDerived>
void InfiniterBit<InfiniterDerived>::setBitUnsafe(uint64_t p_cell_index, uint8_t p_pos) noexcept
{
    this->getData()[p_cell_index] |= (ICELL_C(1) << p_pos);
}

template<typename InfiniterDerived>
void InfiniterBit<InfiniterDerived>::clearBitUnsafe(uint64_t p_cell_index, uint8_t p_pos) noexcept
{
    this->getData()[p_cell_index] &= ~(ICELL_C(1) << p_pos);
}

template<typename InfiniterDerived>
void InfiniterBit<InfiniterDerived>::toggleBitUnsafe(uint64_t p_cell_index, uint8_t p_pos) noexcept
{
    this->getData()[p_cell_index] ^= (ICELL_C(1) << p_pos);
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterBit<InfiniterDerived>::shiftLSB(ibit_t p_lsb)
{
    icell_t lsb_mask = !!p_lsb; /// change 010010...100101 to 000...001

    icell_t *data = this->getData();
    icell_t nextLSB;

    for(isize_t i=0; i<this->getSize(); i++)
    {
        nextLSB = !!(data[i] & M100);
        data[i] = (data[i] << 1) | lsb_mask; /// shift left and set lsb_mask
        lsb_mask = nextLSB;
    }

    /// old MSB is out at this point
    /// overflow possible if that MSB is 1

    this->normalize();
    return this->getRef();
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterBit<InfiniterDerived>::shiftMSB(ibit_t p_msb)
{
    icell_t msb_mask = !!p_msb; /// change 010010...100101 to 00...01

    msb_mask <<= 63; /// change 000...001 to 100...000

    icell_t *data = this->getData();
    icell_t nextMSB;

    for(isize_t i=this->getSize(); i>0; i--)
    {
        nextMSB = (data[i-1] & M001) << 63;
        data[i-1] = (data[i-1] >> 1) | msb_mask; /// shift right and set msb
        msb_mask = nextMSB;
    }

    /// old LSB is out at this point

    this->normalize();
    return this->getRef();
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterBit<InfiniterDerived>::shiftLeft()
{
    return this->shiftLSB(IBIT_0);
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterBit<InfiniterDerived>::shiftRight()
{
    return this->shiftMSB(IBIT_0);
}

template<typename InfiniterDerived>
inline InfiniterDerived &InfiniterBit<InfiniterDerived>::shiftCellsLeft(isize_t p_shift, icell_t p_fill)
{
    isize_t size = this->getRealSize();

    /// if p_fill = 2
    /// Extend required cells
    /// From:           1010 1111 1000
    /// To:   0000 0000 1010 1111 1000
    size = this->setSizeWithExtend(size + p_shift);
    icell_t *data = this->getData(); /// get data after extend (possible allocation)

    /// iterate only over old size (require p_shift size padding)
    /// Shift data with offset
    /// From: 0000 0000 1010 1111 1000
    /// To:   1010 1111 1000 1111 1000
    for(isize_t i=0; i<size - p_shift; i++)
    {
        const isize_t i_rev = size -i -1;
        data[i_rev] = data[]
    }
    /// Clear offset
    /// From: 0000 0000 1010 1111 1000
    /// To:   1010 1111 1000 0000 0000
    for(isize_t i=0; i<new_size - p_shift; i++)
    {
        isize_t i_rev = new_size - i - 1;
        data[i_rev] = data[i_rev - p_shift];
    }

    return this->getRef();
}

template<typename InfiniterDerived>
inline InfiniterDerived &InfiniterBit<InfiniterDerived>::shiftCellsRight(isize_t p_cells)
{

}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterBit<InfiniterDerived>::shiftLeft(isize_t p_operations)
{
    if(p_operations == 0)
    {
        return this->getRef();
    }

    /// well, this could be written with SHLCell and some math
    /// for now it has to be enough
    for(isize_t i=0; i<p_operations-1; i++)
    {
        this->shiftLeft();
    }

    return this->shiftLeft();
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterBit<InfiniterDerived>::shiftRight(isize_t p_operations)
{
    if(p_operations == 0)
    {
        return this->getRef();
    }

    /// well, this could be written with SHRCell and some math
    /// for now it has to be enough
    for(isize_t i=0; i<p_operations-1; i++)
    {
        this->shiftRight();
    }

    return this->shiftRight();
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterBit<InfiniterDerived>::pushLSB(ibit_t p_lsb)
{
    /// check for possible overflow before shifting left
    const isize_t size = this->getSize();
    if(this->getData()[size-1] & M100)
    {
        // MSB is 1 and memory need to be expanded
        this->setSizeWithExtend(size +1);
    }

    return this->shiftLSB(p_lsb);
}

template<typename InfiniterDerived>
InfiniterDerived &InfiniterBit<InfiniterDerived>::pushMSB(ibit_t p_msb)
{
    /// well... adding 0 at the front won't do much
    if(!p_msb)
    {
        this->normalize();
        return this->getRef();
    }

    icell_t *data = this->getData();
    isize_t size = this->getSize();

    /// extend if right now msb is at the left egde
    if(data[size-1] & M100)
    {
        size = this->setSizeWithExtend(size +1);
        data = this->getData();

        data[size-1] = ICELL_C(1);

        this->normalize();
        return this->getRef();
    }

    icell_t *msb_cell = getMSBCellPtr();
    /// if msb is at the edge use next cell
    /// but that means number has leading zero cells
    if(UNLIKELY(*msb_cell & M100))
    {
        msb_cell++;
    }

    int next_bit_index = 64 - __builtin_clzll(*msb_cell);
    *msb_cell |= (ICELL_C(1) << next_bit_index);

    this->normalize();
    return this->getRef();
}

template<typename InfiniterDerived>
inline InfiniterDerived InfiniterBit<InfiniterDerived>::operator ~() const noexcept
{
    return this->getCopy().invert();
}



