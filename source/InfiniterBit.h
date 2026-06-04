#ifndef INFINITERBIT_H
#define INFINITERBIT_H

#include "InfiniterShared.hpp"
#include "InfiniterCore.h"

class Infiniter;

class InfiniterBit : public InfiniterCore
{
public:
    explicit InfiniterBit() noexcept;

    explicit InfiniterBit(uint64_t p_capacity); /// throws bad_alloc
    explicit InfiniterBit(uint64_t p_capacity, uint64_t p_value, bool p_negative_value=false); /// throws bad_alloc
    explicit InfiniterBit(const cell_t *p_array, uint64_t p_size, bool p_negative_value=false); /// throws bad_alloc

    explicit InfiniterBit(const InfiniterBit &p_source); /// throws bad_alloc
    explicit InfiniterBit(InfiniterBit &&p_source) noexcept;

    ~InfiniterBit() noexcept;


    // maybe change all uint64_t to cell where context is abaout bits or cells

    bool checkCellPos(uint64_t cell_id, uint8_t pos);
    void checkCellPosTry(uint64_t cell_id, uint8_t pos); // throws out_of_range


    cell_t getMSBCell() const noexcept;
    cell_t *getMSBCellPtr() const noexcept;
    uint64_t getMSBCellPos() const noexcept;

    cell_t getMSBCell(uint8_t &r_bit_pos) const noexcept;
    cell_t *getMSBCellPtr(uint8_t &r_bit_pos) const noexcept;
    uint64_t getMSBCellPos(uint8_t &r_bit_pos) const noexcept;

    uint8_t getMSBPos() const noexcept;
    uint64_t getMSBGlobalPosUnsafe() const noexcept; // handle edge case
    Infiniter getMSBGlobalPos() const; // throws bad_alloc

    cell_t getMSBPos(cell_t &r_cell) const noexcept;
    cell_t getMSBPos(uint64_t &r_cell_pos) const noexcept;


    uint8_t getBit(uint64_t cell_id, uint8_t pos); // throws out_of_range
    void setBit(uint64_t cell_id, uint8_t pos); // throws out_of_range
    void clearBit(uint64_t cell_id, uint8_t pos); // throws out_of_range
    void toggleBit(uint64_t cell_id, uint8_t pos); // throws out_of_range

    // unsafe or unchecked?
    uint8_t getBitUnsafe(uint64_t cell_id, uint8_t pos) noexcept;
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
