#include "InfiniterBit.hpp"

#include "InfiniterShared.hpp"

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

InfiniterBit::InfiniterBit(uint64_t p_capacity)
    : InfiniterCore(p_capacity)
{
    _ib_dbgprintf("--- DEBUG IB %p | Constructed   PARAMETER 1\n", this);
}

InfiniterBit::InfiniterBit(uint64_t p_capacity, uint64_t p_value, bool p_negative_value)
    : InfiniterCore(p_capacity, p_value, p_negative_value)
{
    _ib_dbgprintf("--- DEBUG IB %p | Constructed   PARAMETER 2\n", this);
}

InfiniterBit::InfiniterBit(const cell_t *p_array, uint64_t p_size, bool p_negative_value)
    : InfiniterCore(p_array, p_size, p_negative_value)
{
    _ib_dbgprintf("--- DEBUG IB %p | Constructed   PARAMETER 3\n", this);
}

InfiniterBit::InfiniterBit(const InfiniterBit &p_source)
    : InfiniterCore(p_source)
{
    _ib_dbgprintf("--- DEBUG IB %p | Constructed   COPY\n", this);
}

InfiniterBit::InfiniterBit(InfiniterBit &&p_source) noexcept
    : InfiniterCore(std::move(p_source))
{
    _ib_dbgprintf("--- DEBUG IB %p | Constructed   MOVE\n", this);
}

InfiniterBit::~InfiniterBit() noexcept
{
    _ib_dbgprintf("--- DEBUG IB %p | Delete\n", this);

    /// everything was done in InfiniterMemory
}

bool InfiniterBit::checkCellPos(uint64_t cell_id, uint8_t pos)
{
    return cell_id < this->getSize() && pos < 64;
}

void InfiniterBit::checkCellPosTry(uint64_t cell_id, uint8_t pos)
{
    if(cell_id >= this->getSize() || pos >= 64)
    {
        throw std::out_of_range(
            "Range error: cell_id " + std::to_string(cell_id) +
            " (max < " + std::to_string(this->getSize()) +
            "), pos " + std::to_string(static_cast<int>(pos)) +
            " (max < 64)"
        );
    }
}

cell_t InfiniterBit::getMSBCell() const noexcept
{
    const cell_t *const data = this->getData();
    const cell_t *cellPtr = data + this->getSize(); /// intentional out of bounds

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return its value
    do{
        if(*(--cellPtr) != UINT64_C(0))
            return *cellPtr;
    }
    while(cellPtr != data);

    /// in any case (0 or non 0 value) return least significant cell value
    return *cellPtr;
}

const cell_t *InfiniterBit::getMSBCellPtr() const noexcept
{
    const cell_t *const data = this->getData();
    const cell_t *cellPtr = data + this->getSize(); /// intentional out of bounds

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return the cell ptr
    do{
        if(*(--cellPtr) != UINT64_C(0))
        {
            return cellPtr;
        }
    }
    while(cellPtr != data);

    /// in any case (0 or non 0 value) return least significant cell
    return cellPtr;
}

cell_t *InfiniterBit::getMSBCellPtr() noexcept
{
    cell_t *const data = this->getData();
    cell_t *cellPtr = data + this->getSize(); /// intentional out of bounds

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return the cell ptr
    do{
        if(*(--cellPtr) != UINT64_C(0))
        {
            return cellPtr;
        }
    }
    while(cellPtr != data);

    /// in any case (0 or non 0 value) return least significant cell
    return cellPtr;
}

uint64_t InfiniterBit::getMSBCellPos() const noexcept
{
    uint64_t size = this->getSize();
    const cell_t *data = this->getData();

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return its index
    for(uint64_t i=0; i<size; i++)
    {
        const uint64_t pos = size -1 -i;
        if(data[pos] != UINT64_C(0))
        {
            return pos;
        }
    }

    /// in any case (0 or non 0 value) return least significant cell index
    return 0;
}

cell_t InfiniterBit::getMSBCell(uint8_t &r_bit_pos) const noexcept
{
    const cell_t *const data = this->getData();
    const cell_t *cellPtr = data + this->getSize(); /// intentional out of bounds

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return its value
    do{
        if(*(--cellPtr) != UINT64_C(0))
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

const cell_t *InfiniterBit::getMSBCellPtr(uint8_t &r_bit_pos) const noexcept
{
    const cell_t *const data = this->getData();
    const cell_t *cellPtr = data + this->getSize(); /// intentional out of bounds

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return its value
    do{
        if(*(--cellPtr) != UINT64_C(0))
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

cell_t *InfiniterBit::getMSBCellPtr(uint8_t &r_bit_pos) noexcept
{
    cell_t *const data = this->getData();
    cell_t *cellPtr = data + this->getSize(); /// intentional out of bounds

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return its value
    do{
        if(*(--cellPtr) != UINT64_C(0))
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

uint64_t InfiniterBit::getMSBCellPos(uint8_t &r_bit_pos) const noexcept
{
    uint64_t size = this->getSize();
    const cell_t *data = this->getData();

    /// iterate from most significant cell, to least significant
    /// until non 0 cell found, then return its index
    for(uint64_t i=0; i<size; i++)
    {
        const uint64_t pos = size -1 -i;
        if(data[pos] != UINT64_C(0))
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

}

uint64_t InfiniterBit::getMSBGlobalPosUnsafe() const noexcept // handle edge case
{

}

Infiniter InfiniterBit::getMSBGlobalPos() const // throws bad_alloc
{

}

uint8_t InfiniterBit::getMSBPos(cell_t &r_cell) const noexcept
{

}

uint8_t InfiniterBit::getMSBPos(uint64_t *&r_cell_ptr) const noexcept
{

}

uint8_t InfiniterBit::getMSBPosCellPos(uint64_t &r_cell_pos) const noexcept
{

}





cell_t InfiniterBit::getLSBCell() const noexcept
{

}

const cell_t *InfiniterBit::getLSBCellPtr() const noexcept
{

}

cell_t *InfiniterBit::getLSBCellPtr() noexcept
{

}

uint64_t InfiniterBit::getLSBCellPos() const noexcept
{

}

cell_t InfiniterBit::getLSBCell(uint8_t &r_bit_pos) const noexcept
{

}

const cell_t *InfiniterBit::getLSBCellPtr(uint8_t &r_bit_pos) const noexcept
{

}

cell_t *InfiniterBit::getLSBCellPtr(uint8_t &r_bit_pos) noexcept
{

}

uint64_t InfiniterBit::getLSBCellPos(uint8_t &r_bit_pos) const noexcept
{

}

uint8_t InfiniterBit::getLSBPos() const noexcept
{

}

uint64_t InfiniterBit::getLSBGlobalPosUnsafe() const noexcept // handle edge case
{

}

Infiniter InfiniterBit::getLSBGlobalPos() const // throws bad_alloc
{

}

uint8_t InfiniterBit::getLSBPos(cell_t &r_cell) const noexcept
{

}

uint8_t InfiniterBit::getLSBPos(uint64_t *&r_cell_ptr) const noexcept
{

}

uint8_t InfiniterBit::getLSBPosCellPos(uint64_t &r_cell_pos) const noexcept
{

}







uint8_t InfiniterBit::getBit(uint64_t cell_id, uint8_t pos)
{
    this->checkCellPosTry(cell_id, pos);
    
    return this->getData()[cell_id] & (UINT64_C(1) << pos) ? UINT8_C(1) : UINT8_C(0);
}

void InfiniterBit::setBit(uint64_t cell_id, uint8_t pos)
{
    this->checkCellPosTry(cell_id, pos);
    
    this->getData()[cell_id] |= (UINT64_C(1) << pos);
}

void InfiniterBit::clearBit(uint64_t cell_id, uint8_t pos)
{
    this->checkCellPosTry(cell_id, pos);

    this->getData()[cell_id] &= ~(UINT64_C(1) << pos);
}

void InfiniterBit::toggleBit(uint64_t cell_id, uint8_t pos)
{
    this->checkCellPosTry(cell_id, pos);

    this->getData()[cell_id] ^= (UINT64_C(1) << pos);
}

uint8_t InfiniterBit::getBitUnsafe(uint64_t cell_id, uint8_t pos) noexcept
{
    return this->getData()[cell_id] & (UINT64_C(1) << pos) ? UINT8_C(1) : UINT8_C(0);
}

void InfiniterBit::setBitUnsafe(uint64_t cell_id, uint8_t pos) noexcept
{
    this->getData()[cell_id] |= (UINT64_C(1) << pos);
}

void InfiniterBit::clearBitUnsafe(uint64_t cell_id, uint8_t pos) noexcept
{
    this->getData()[cell_id] &= ~(UINT64_C(1) << pos);
}

void InfiniterBit::toggleBitUnsafe(uint64_t cell_id, uint8_t pos) noexcept
{
    this->getData()[cell_id] ^= (UINT64_C(1) << pos);
}

void InfiniterBit::shiftLSB(uint64_t lsb)
{
    // optional safe addition
    lsb = !!lsb; // change 010010...100101 to 000...001

    cell_t *data = this->getData();
    uint64_t nextLSB;

    for(uint64_t i=0; i<this->getSize(); i++)
    {
        nextLSB = !!(data[i] & M100);
        data[i] = (data[i] << 1) | lsb; // shift left and set lsb
        lsb = nextLSB;
    }

    // old MSB is out at this point
    // overflow possible if that MSB is 1
}

void InfiniterBit::shiftMSB(uint64_t msb)
{
    // optional safe addition
    msb = !!msb; // change 010010...100101 to 00...01

    msb <<= 63; // change 00...01 to 10...00

    cell_t *data = this->getData();
    uint64_t nextMSB;

    for(uint64_t i=this->getSize(); i>0; i--)
    {
        nextMSB = (data[i-1] & M001) << 63;
        data[i-1] = (data[i-1] >> 1) | msb; // shift right and set msb
        msb = nextMSB;
    }

    // old LSB is out at this point
}

void InfiniterBit::shiftLeft()
{
    this->shiftLSB(0);
}

void InfiniterBit::shiftRight()
{
    this->shiftMSB(0);
}

void InfiniterBit::pushLSB(uint64_t lsb)
{
    /// check for possible overflow before shifting left
    const uint64_t size = this->getSize();
    if(this->getData()[size-1] & M100)
    {
        // MSB is 1 and memory need to be expanded
        this->setSizeWithExtend(size +1);
    }

    this->shiftLSB(lsb);
}

void InfiniterBit::pushMSB(uint64_t msb)
{
    cell_t *data = this->getData();
    uint64_t size = this->getSize();
    if(data[size-1] & M100)
    {
        size = this->setSizeWithExtend(size +1);
    }




    this->shiftMSB(msb);
}

