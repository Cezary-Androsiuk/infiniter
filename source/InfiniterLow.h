#ifndef INFINITERLOW_H
#define INFINITERLOW_H

#include "InfiniterCore.h"

class InfiniterLow : public InfiniterCore
{
public:
    InfiniterLow();

    void checkCellPos(uint64_t cell_id, uint8_t pos);

    uint8_t getBit(uint64_t cell_id, uint8_t pos);
    void setBit(uint64_t cell_id, uint8_t pos);
    void clearBit(uint64_t cell_id, uint8_t pos);
    void toggleBit(uint64_t cell_id, uint8_t pos);

    // unsafe or unchecked?
    uint8_t getBitUnsafe(uint64_t cell_id, uint8_t pos) noexcept;
    void setBitUnsafe(uint64_t cell_id, uint8_t pos) noexcept;
    void clearBitUnsafe(uint64_t cell_id, uint8_t pos) noexcept;
    void toggleBitUnsafe(uint64_t cell_id, uint8_t pos) noexcept;
    

    

};

#endif // INFINITERLOW_H
