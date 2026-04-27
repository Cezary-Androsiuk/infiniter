#ifndef INFINITERBIT_H
#define INFINITERBIT_H

#include "InfiniterIO.h"

#include "InfiniterCommon.hpp"

class InfiniterBit : public InfiniterIO
{
public:
    InfiniterBit();

    // maybe change all uint64_t to cell where context is abaout bits or cells

    void checkCellPos(uint64_t cell_id, uint8_t pos);

    uint64_t getBit(uint64_t cell_id, uint8_t pos);
    void setBit(uint64_t cell_id, uint8_t pos);
    void clearBit(uint64_t cell_id, uint8_t pos);
    void toggleBit(uint64_t cell_id, uint8_t pos);

    // unsafe or unchecked?
    uint64_t getBitUnsafe(uint64_t cell_id, uint8_t pos) noexcept;
    void setBitUnsafe(uint64_t cell_id, uint8_t pos) noexcept;
    void clearBitUnsafe(uint64_t cell_id, uint8_t pos) noexcept;
    void toggleBitUnsafe(uint64_t cell_id, uint8_t pos) noexcept;

    void shiftLSB(uint64_t lsb=0); // Least Significant Bit (<<)
    void shiftMSB(uint64_t msb=0); // Most Significant Bit (>>)

    void shiftLeft();
    void shiftRight();

    void pushLSB(uint64_t lsb);
    void pushMSB(uint64_t msb);
    

};

#endif // INFINITERBIT_H
