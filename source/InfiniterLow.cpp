#include "InfiniterLow.h"

InfiniterLow::InfiniterLow()
    : InfiniterCore()
{

}

void InfiniterLow::checkCellPos(uint64_t cell_id, uint8_t pos)
{
    if(cell_id >= this->getSize() || pos > 63)
        throw std::out_of_range;
}

uint8_t InfiniterLow::getBit(uint64_t cell_id, uint8_t pos)
{
    this->checkCellPos(cell_id, pos);
    
    return this->getData()[cell_id] & (UINT64(1) << pos);
}

void InfiniterLow::setBit(uint64_t cell_id, uint8_t pos)
{
    this->checkCellPos(cell_id, pos);
    
    this->getData()[cell_id] |= (UINT64(1) << pos);
}

void InfiniterLow::clearBit(uint64_t cell_id, uint8_t pos)
{
    this->checkCellPos(cell_id, pos);

    this->getData()[cell_id] &= ~(UINT64(1) << pos);
}

void toggleBit(uint64_t cell_id, uint8_t pos)
{
    this->checkCellPos(cell_id, pos);

    this->getData()[cell_id] ^= (UINT64(1) << pos);
}

uint8_t InfiniterLow::getBitUnsafe(uint64_t cell_id, uint8_t pos) noexcept
{
    return this->getData()[cell_id] & (UINT64(1) << pos);
}

void InfiniterLow::setBitUnsafe(uint64_t cell_id, uint8_t pos) noexcept
{
    this->getData()[cell_id] |= (UINT64(1) << pos);
}

void InfiniterLow::clearBitUnsafe(uint64_t cell_id, uint8_t pos) noexcept
{
    this->getData()[cell_id] &= ~(UINT64(1) << pos);
}

void toggleBitUnsafe(uint64_t cell_id, uint8_t pos) noexcept
{
    this->getData()[cell_id] ^= (UINT64(1) << pos);
}
