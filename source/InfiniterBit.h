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

    /// InfiniterCore methods

    // maybe change all uint64_t to cell where context is abaout bits or cells

    bool checkCellPos(uint64_t cell_id, uint8_t pos);
    void checkCellPosTry(uint64_t cell_id, uint8_t pos); // throws out_of_range


    /// Returns value of cell with MSB
    cell_t getMSBCell() const noexcept;
    /// Returns const pointer to cell with MSB
    const cell_t *getMSBCellPtr() const noexcept;
    /// Returns pointer to cell with MSB
    cell_t *getMSBCellPtr() noexcept;
    /// Returns position of cell with MSB
    uint64_t getMSBCellPos() const noexcept;

    /// Returns value of cell with MSB and get MSB position in cell
    cell_t getMSBCell(uint8_t &r_bit_pos) const noexcept;
    /// Returns const pointer to cell with MSB and get MSB position in cell
    const cell_t *getMSBCellPtr(uint8_t &r_bit_pos) const noexcept;
    /// Returns pointer to cell with MSB and get MSB position in cell
    cell_t *getMSBCellPtr(uint8_t &r_bit_pos) noexcept;
    /// Returns position of cell with MSB and get MSB position in cell
    uint64_t getMSBCellPos(uint8_t &r_bit_pos) const noexcept;

    /// Returns MSB position in cell
    uint8_t getMSBPos() const noexcept;
    /// Returns MSB global position in cell  (could fail for large numbers)
    uint64_t getMSBGlobalPosUnsafe() const noexcept; // handle edge case
    /// Returns MSB global position in cell
    Infiniter getMSBGlobalPos() const; // throws bad_alloc

    /// Returns MSB position in cell and get MSB cell value
    uint8_t getMSBPos(cell_t &r_cell) const noexcept;
    /// Returns MSB position in cell and get MSB cell pointer
    uint8_t getMSBPos(uint64_t *&r_cell_ptr) const noexcept;
    /// Returns MSB position in cell and get MSB cell position
    uint8_t getMSBPosCellPos(uint64_t &r_cell_pos) const noexcept;



    /// Returns value of cell with LSB
    cell_t getLSBCell() const noexcept;
    /// Returns const pointer to cell with LSB
    const cell_t *getLSBCellPtr() const noexcept;
    /// Returns pointer to cell with LSB
    cell_t *getLSBCellPtr() noexcept;
    /// Returns position of cell with LSB
    uint64_t getLSBCellPos() const noexcept;

    /// Returns value of cell with LSB and get LSB position in cell
    cell_t getLSBCell(uint8_t &r_bit_pos) const noexcept;
    /// Returns const pointer to cell with LSB and get LSB position in cell
    const cell_t *getLSBCellPtr(uint8_t &r_bit_pos) const noexcept;
    /// Returns pointer to cell with LSB and get LSB position in cell
    cell_t *getLSBCellPtr(uint8_t &r_bit_pos) noexcept;
    /// Returns position of cell with LSB and get LSB position in cell
    uint64_t getLSBCellPos(uint8_t &r_bit_pos) const noexcept;

    /// Returns LSB position in cell
    uint8_t getLSBPos() const noexcept;
    /// Returns LSB global position in cell  (could fail for large numbers)
    uint64_t getLSBGlobalPosUnsafe() const noexcept; // handle edge case
    /// Returns LSB global position in cell
    Infiniter getLSBGlobalPos() const; // throws bad_alloc

    /// Returns LSB position in cell and get LSB cell value
    uint8_t getLSBPos(cell_t &r_cell) const noexcept;
    /// Returns LSB position in cell and get LSB cell pointer
    uint8_t getLSBPos(uint64_t *&r_cell_ptr) const noexcept;
    /// Returns LSB position in cell and get LSB cell position
    uint8_t getLSBPosCellPos(uint64_t &r_cell_pos) const noexcept;



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
