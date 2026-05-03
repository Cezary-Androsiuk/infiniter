#include "InfiniterBit.h"

#include <stdexcept>
#include <sstream>

#include "InfiniterCommon.hpp"

InfiniterBit::InfiniterBit()
    : InfiniterIO()
{

}

void InfiniterBit::checkCellPos(uint64_t cell_id, uint8_t pos)
{
    if(cell_id >= this->getSize() || pos >= 64)
    {
        std::ostringstream ss;
        ss << "Range error: cell_id " << cell_id << " (max < " << this->getSize()
            << "), pos " << static_cast<int>(pos) << " (max < 64)";

        throw std::out_of_range( ss.str() );
    }
}

uint64_t InfiniterBit::getBit(uint64_t cell_id, uint8_t pos)
{
    this->checkCellPos(cell_id, pos);
    
    return this->getData()[cell_id] & (UINT64_C(1) << pos);
}

void InfiniterBit::setBit(uint64_t cell_id, uint8_t pos)
{
    this->checkCellPos(cell_id, pos);
    
    this->getData()[cell_id] |= (UINT64_C(1) << pos);
}

void InfiniterBit::clearBit(uint64_t cell_id, uint8_t pos)
{
    this->checkCellPos(cell_id, pos);

    this->getData()[cell_id] &= ~(UINT64_C(1) << pos);
}

void InfiniterBit::toggleBit(uint64_t cell_id, uint8_t pos)
{
    this->checkCellPos(cell_id, pos);

    this->getData()[cell_id] ^= (UINT64_C(1) << pos);
}

uint64_t InfiniterBit::getBitUnsafe(uint64_t cell_id, uint8_t pos) noexcept
{
    return this->getData()[cell_id] & (UINT64_C(1) << pos);
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
    lsb = !!lsb; // change 010010...100101 to 00...01

    cell *data = this->getData();
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

    cell *data = this->getData();
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
    if(this->getData()[this->getSize()-1] & M100)
    {
        // MSB is 1 and memory need to be expanded

    }

    this->shiftLSB(lsb);
}

void InfiniterBit::pushMSB(uint64_t msb)
{
    this->shiftMSB(msb);
}

