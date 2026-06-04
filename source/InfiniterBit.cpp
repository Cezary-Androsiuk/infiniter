#include "InfiniterBit.h"

#include "InfiniterShared.hpp"

#include <stdexcept>
#include <sstream>

InfiniterBit::InfiniterBit() noexcept
    : InfiniterCore()
{

}

InfiniterBit::InfiniterBit(uint64_t p_capacity)
    : InfiniterCore(p_capacity)
{

}

InfiniterBit::InfiniterBit(uint64_t p_capacity, uint64_t p_value, bool p_negative_value)
    : InfiniterCore(p_capacity, p_value, p_negative_value)
{

}

InfiniterBit::InfiniterBit(const cell_t *p_array, uint64_t p_size, bool p_negative_value)
    : InfiniterCore(p_array, p_size, p_negative_value)
{

}

InfiniterBit::InfiniterBit(const InfiniterBit &p_source)
{

}

InfiniterBit::InfiniterBit(InfiniterBit &&p_source) noexcept
{

}

InfiniterBit::~InfiniterBit() noexcept
{

}

bool InfiniterBit::checkCellPos(uint64_t cell_id, uint8_t pos)
{
    return cell_id < this->getSize() && pos < 64;
}

void InfiniterBit::checkCellPosTry(uint64_t cell_id, uint8_t pos)
{
    if(cell_id >= this->getSize() || pos >= 64)
    {
        // std::ostringstream ss;
        // ss << "Range error: cell_id " << cell_id << " (max < " << this->getSize()
        //     << "), pos " << static_cast<int>(pos) << " (max < 64)";

        // throw std::out_of_range( ss.str() );
        throw std::out_of_range(
            "Range error: cell_id " + std::to_string(cell_id) +
            " (max < " + std::to_string(this->getSize()) +
            "), pos " + std::to_string(static_cast<int>(pos)) +
            " (max < 64)"
        );
    }
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

