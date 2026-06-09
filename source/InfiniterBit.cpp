#include "InfiniterBit.hpp"

#include "InfiniterShared.hpp"
#include "InfiniterException.hpp"

#include "Infiniter.hpp"

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

InfiniterBit::InfiniterBit() noexcept
    : InfiniterCore()
{
    _ib_dbgprintf("--- DEBUG IB %p | Constructed   DEFAULT\n", this);
}

InfiniterBit::InfiniterBit(isize_t p_capacity)
    : InfiniterCore(p_capacity)
{
    _ib_dbgprintf("--- DEBUG IB %p | Constructed   PARAMETER isize_t\n", this);
}

InfiniterBit::InfiniterBit(isize_t p_capacity, icell_t p_value, bool p_negative_value)
    : InfiniterCore(p_capacity, p_value, p_negative_value)
{
    _ib_dbgprintf("--- DEBUG IB %p | Constructed   PARAMETER isize_t icell_t bool\n", this);
}

InfiniterBit::InfiniterBit(const icell_t *p_array, isize_t p_size, bool p_negative_value)
    : InfiniterCore(p_array, p_size, p_negative_value)
{
    _ib_dbgprintf("--- DEBUG IB %p | Constructed   PARAMETER cell_t* isize_t bool\n", this);
}

InfiniterBit::InfiniterBit(const InfiniterBit &p_source)
    : InfiniterCore(p_source)
{
    _ib_dbgprintf("--- DEBUG IB %p | Constructed IB COPY\n", this);
}

InfiniterBit::InfiniterBit(InfiniterBit &&p_source) noexcept
    : InfiniterCore(std::move(p_source))
{
    _ib_dbgprintf("--- DEBUG IB %p | Constructed IB MOVE\n", this);
}

InfiniterBit::InfiniterBit(const InfiniterCore &p_source)
    : InfiniterCore(p_source)
{
    _ib_dbgprintf("--- DEBUG IB %p | Constructed IC COPY\n", this);
}

InfiniterBit::InfiniterBit(InfiniterCore &&p_source) noexcept
    : InfiniterCore(std::move(p_source))
{
    _ib_dbgprintf("--- DEBUG IB %p | Constructed IC MOVE\n", this);
}

InfiniterBit::~InfiniterBit() noexcept
{
    _ib_dbgprintf("--- DEBUG IB %p | Delete\n", this);

    /// everything was done in InfiniterMemory
}

bool InfiniterBit::checkCellPos(isize_t cell_id, uint8_t pos) const
{
    return cell_id < this->getSize() && pos < 64;
}

void InfiniterBit::checkCellPosTry(isize_t cell_id, uint8_t pos) const
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

icell_t InfiniterBit::getMSBCell() const noexcept
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

const icell_t *InfiniterBit::getMSBCellPtr() const noexcept
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

icell_t *InfiniterBit::getMSBCellPtr() noexcept
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

isize_t InfiniterBit::getMSBCellPos() const noexcept
{
    isize_t size = this->getSize();
    const icell_t *data = this->getData();

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return its index
    for(isize_t i=0; i<size; i++)
    {
        const isize_t pos = size -1 -i;
        if(data[pos] != ICELL_C(0))
        {
            return pos;
        }
    }

    /// in any case (0 or non 0 value) return least significant cell index
    return 0;
}

icell_t InfiniterBit::getMSBCell(uint8_t &r_bit_pos) const noexcept
{
    const icell_t *const data = this->getData();
    const icell_t *cellPtr = data + this->getSize(); /// intentional out of bounds

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return its value
    do{
        if(*(--cellPtr) != ICELL_C(0))
        {
            /// find MSB in cell
            /// clzll stands for "Count Leading Zeros Long Long"
            r_bit_pos = 63 - __builtin_clzll(*cellPtr);

            return *cellPtr;
        }
    }
    while(cellPtr != data);

    /// in any case (0 or non 0 value) return least significant cell value
    /// but first find MSB in cell
    /// clzll stands for "Count Leading Zeros Long Long"
    r_bit_pos = 63 - __builtin_clzll(*cellPtr);

    return *cellPtr;
}

const icell_t *InfiniterBit::getMSBCellPtr(uint8_t &r_bit_pos) const noexcept
{
    const icell_t *const data = this->getData();
    const icell_t *cellPtr = data + this->getSize(); /// intentional out of bounds

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return its value
    do{
        if(*(--cellPtr) != ICELL_C(0))
        {
            /// find MSB in cell
            /// clzll stands for "Count Leading Zeros Long Long"
            r_bit_pos = 63 - __builtin_clzll(*cellPtr);

            return cellPtr;
        }
    }
    while(cellPtr != data);

    /// in any case (0 or non 0 value) return least significant cell value
    /// but first find MSB in cell
    /// clzll stands for "Count Leading Zeros Long Long"
    r_bit_pos = 63 - __builtin_clzll(*cellPtr);

    return cellPtr;
}

icell_t *InfiniterBit::getMSBCellPtr(uint8_t &r_bit_pos) noexcept
{
    icell_t *const data = this->getData();
    icell_t *cellPtr = data + this->getSize(); /// intentional out of bounds

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return its value
    do{
        if(*(--cellPtr) != ICELL_C(0))
        {
            /// find MSB in cell
            /// clzll stands for "Count Leading Zeros Long Long"
            r_bit_pos = 63 - __builtin_clzll(*cellPtr);

            return cellPtr;
        }
    }
    while(cellPtr != data);

    /// in any case (0 or non 0 value) return least significant cell value
    /// but first find MSB in cell
    /// clzll stands for "Count Leading Zeros Long Long"
    r_bit_pos = 63 - __builtin_clzll(*cellPtr);

    return cellPtr;
}

isize_t InfiniterBit::getMSBCellPos(uint8_t &r_bit_pos) const noexcept
{
    isize_t size = this->getSize();
    const icell_t *data = this->getData();

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return its index
    for(isize_t i=0; i<size; i++)
    {
        const isize_t pos = size -1 -i;
        if(data[pos] != ICELL_C(0))
        {
            /// find MSB in cell
            /// clzll stands for "Count Leading Zeros Long Long"
            r_bit_pos = 63 - __builtin_clzll(data[pos]);

            return pos;
        }
    }

    /// in any case (0 or non 0 value) return least significant cell index
    /// find MSB in cell
    /// clzll stands for "Count Leading Zeros Long Long"
    r_bit_pos = 63 - __builtin_clzll(data[0]);

    return 0;
}

uint8_t InfiniterBit::getMSBPos() const noexcept
{
    const icell_t *const data = this->getData();
    const icell_t *cellPtr = data + this->getSize(); /// intentional out of bounds

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return its value
    do{
        if(*(--cellPtr) != ICELL_C(0))
        {
            /// find MSB in cell
            /// clzll stands for "Count Leading Zeros Long Long"
            return 63 - __builtin_clzll(*cellPtr);
        }
    }
    while(cellPtr != data);

    /// in any case (0 or non 0 value) return least significant cell value
    /// find MSB in that cell
    /// clzll stands for "Count Leading Zeros Long Long"
    return 63 - __builtin_clzll(*cellPtr);
}

isize_t InfiniterBit::getMSBGlobalPosUnsafe() const noexcept // handle edge case
{

}

Infiniter InfiniterBit::getMSBGlobalPos() const // throws bad_alloc
{

}

uint8_t InfiniterBit::getMSBPos(icell_t &r_cell) const noexcept
{

}

uint8_t InfiniterBit::getMSBPos(icell_t *&r_cell_ptr) const noexcept
{

}

uint8_t InfiniterBit::getMSBPosCellPos(isize_t &r_cell_pos) const noexcept
{

}





icell_t InfiniterBit::getLSBCell() const noexcept
{

}

const icell_t *InfiniterBit::getLSBCellPtr() const noexcept
{

}

icell_t *InfiniterBit::getLSBCellPtr() noexcept
{

}

isize_t InfiniterBit::getLSBCellPos() const noexcept
{

}

icell_t InfiniterBit::getLSBCell(uint8_t &r_bit_pos) const noexcept
{

}

const icell_t *InfiniterBit::getLSBCellPtr(uint8_t &r_bit_pos) const noexcept
{

}

icell_t *InfiniterBit::getLSBCellPtr(uint8_t &r_bit_pos) noexcept
{

}

isize_t InfiniterBit::getLSBCellPos(uint8_t &r_bit_pos) const noexcept
{

}

uint8_t InfiniterBit::getLSBPos() const noexcept
{

}

isize_t InfiniterBit::getLSBGlobalPosUnsafe() const noexcept // handle edge case
{

}

Infiniter InfiniterBit::getLSBGlobalPos() const // throws bad_alloc
{

}

uint8_t InfiniterBit::getLSBPos(icell_t &r_cell) const noexcept
{

}

uint8_t InfiniterBit::getLSBPos(icell_t *&r_cell_ptr) const noexcept
{

}

uint8_t InfiniterBit::getLSBPosCellPos(isize_t &r_cell_pos) const noexcept
{

}







ibit_t InfiniterBit::getBit(isize_t p_cell_index, uint8_t p_pos)
{
    this->checkCellPosTry(p_cell_index, p_pos);
    
    return this->getData()[p_cell_index] & (ICELL_C(1) << p_pos) ? IBIT_TRUE : IBIT_FALSE;
}

void InfiniterBit::setBit(isize_t p_cell_index, uint8_t p_pos)
{
    this->checkCellPosTry(p_cell_index, p_pos);
    
    this->getData()[p_cell_index] |= (ICELL_C(1) << p_pos);
}

void InfiniterBit::clearBit(isize_t p_cell_index, uint8_t p_pos)
{
    this->checkCellPosTry(p_cell_index, p_pos);

    this->getData()[p_cell_index] &= ~(ICELL_C(1) << p_pos);
}

void InfiniterBit::toggleBit(isize_t p_cell_index, uint8_t p_pos)
{
    this->checkCellPosTry(p_cell_index, p_pos);

    this->getData()[p_cell_index] ^= (ICELL_C(1) << p_pos);
}

ibit_t InfiniterBit::getBitUnsafe(uint64_t p_cell_index, uint8_t p_pos) noexcept
{
    return this->getData()[p_cell_index] & (ICELL_C(1) << p_pos) ? IBIT_TRUE : IBIT_FALSE;
}

void InfiniterBit::setBitUnsafe(uint64_t p_cell_index, uint8_t p_pos) noexcept
{
    this->getData()[p_cell_index] |= (ICELL_C(1) << p_pos);
}

void InfiniterBit::clearBitUnsafe(uint64_t p_cell_index, uint8_t p_pos) noexcept
{
    this->getData()[p_cell_index] &= ~(ICELL_C(1) << p_pos);
}

void InfiniterBit::toggleBitUnsafe(uint64_t p_cell_index, uint8_t p_pos) noexcept
{
    this->getData()[p_cell_index] ^= (ICELL_C(1) << p_pos);
}

void InfiniterBit::shiftLSB(ibit_t p_lsb)
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
}

void InfiniterBit::shiftMSB(ibit_t p_msb)
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
}

void InfiniterBit::shiftLeft()
{
    this->shiftLSB(IBIT_0);
}

void InfiniterBit::shiftRight()
{
    this->shiftMSB(IBIT_0);
}

void InfiniterBit::pushLSB(ibit_t p_lsb)
{
    /// check for possible overflow before shifting left
    const isize_t size = this->getSize();
    if(this->getData()[size-1] & M100)
    {
        // MSB is 1 and memory need to be expanded
        this->setSizeWithExtend(size +1);
    }

    this->shiftLSB(p_lsb);
}

void InfiniterBit::pushMSB(ibit_t p_msb)
{
    /// well... adding 0 at the front won't do much
    if(!p_msb)
        return;

    icell_t *data = this->getData();
    isize_t size = this->getSize();

    /// extend if right now msb is at the left egde
    if(data[size-1] & M100)
    {
        size = this->setSizeWithExtend(size +1);
        data = this->getData();

        data[size-1] = ICELL_C(1);
        return;
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
}

void InfiniterBit::assign(const InfiniterBit &p_source)
{
    InfiniterCore::assign(p_source);
}

void InfiniterBit::assign(InfiniterBit &&p_source)
{
    InfiniterCore::assign(std::move(p_source));
}

InfiniterBit &InfiniterBit::operator =(const InfiniterBit &p_source)
{
    InfiniterCore::assign(p_source);
    return *this;
}

InfiniterBit &InfiniterBit::operator =(InfiniterBit &&p_source)
{
    InfiniterCore::assign(std::move(p_source));
    return *this;
}



