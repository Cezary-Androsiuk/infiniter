#pragma once

#include "InfiniterShared.hpp"
#include "InfiniterCore.hpp"

class Infiniter;

template <typename InfiniterDerived>
class InfiniterBit : public InfiniterCore<InfiniterDerived>
{
public:
    inline explicit InfiniterBit() noexcept;

    inline InfiniterBit(int64_t p_value); /// throws bad_alloc
    inline explicit InfiniterBit(int64_t p_value, isize_t p_capacity); /// throws bad_alloc
    inline explicit InfiniterBit(icell_t p_value, isize_t p_capacity, bool p_negative_value); /// throws bad_alloc
    inline explicit InfiniterBit(const icell_t *p_array, isize_t p_size, bool p_negative_value=false); /// throws bad_alloc

    inline InfiniterBit(const InfiniterDerived &p_source); /// throws bad_alloc
    inline InfiniterBit(InfiniterDerived &&p_source) noexcept;

    inline ~InfiniterBit() noexcept;

    /// InfiniterCore methods

    // maybe change all uint64_t to cell where context is abaout bits or cells

    inline bool checkCellPos(isize_t cell_id, uint8_t pos) const;
    void checkCellPosTry(isize_t cell_id, uint8_t pos) const; // throws out_of_range


    /// Returns value of cell with MSB
    icell_t getMSBCell() const noexcept;
    /// Returns const pointer to cell with MSB
    const icell_t *getMSBCellPtr() const noexcept;
    /// Returns pointer to cell with MSB
    icell_t *getMSBCellPtr() noexcept;
    /// Returns position of cell with MSB
    isize_t getMSBCellPos() const noexcept;

    /// Returns value of cell with MSB and get MSB position in cell
    icell_t getMSBCell(uint8_t &r_bit_pos) const noexcept;
    /// Returns const pointer to cell with MSB and get MSB position in cell
    const icell_t *getMSBCellPtr(uint8_t &r_bit_pos) const noexcept;
    /// Returns pointer to cell with MSB and get MSB position in cell
    icell_t *getMSBCellPtr(uint8_t &r_bit_pos) noexcept;
    /// Returns position of cell with MSB and get MSB position in cell
    isize_t getMSBCellPos(uint8_t &r_bit_pos) const noexcept;

    /// Returns const pointer to cell with MSB and position of cell
    const icell_t *getMSBCellPtr(isize_t &r_pos) const noexcept;
    /// Returns pointer to cell with MSB and position of cell
    icell_t *getMSBCellPtr(isize_t &r_pos) noexcept;

    /// Returns MSB position in cell
    uint8_t getMSBPos() const noexcept;/////////////////////////////////////////////////// done here
    /// Returns MSB global position in cell  (could fail for large numbers)
    isize_t getMSBGlobalPos(isize_t *const overflow = nullptr) const noexcept;
    /// Returns MSB global position in cell
    InfiniterDerived getMSBGlobalPos() const; // throws bad_alloc



    /// Returns value of cell with LSB
    icell_t getLSBCell() const noexcept;
    /// Returns const pointer to cell with LSB
    const icell_t *getLSBCellPtr() const noexcept;
    /// Returns pointer to cell with LSB
    icell_t *getLSBCellPtr() noexcept;
    /// Returns position of cell with LSB
    isize_t getLSBCellPos() const noexcept;

    /// Returns value of cell with LSB and get LSB position in cell
    icell_t getLSBCell(uint8_t &r_bit_pos) const noexcept;
    /// Returns const pointer to cell with LSB and get LSB position in cell
    const icell_t *getLSBCellPtr(uint8_t &r_bit_pos) const noexcept;
    /// Returns pointer to cell with LSB and get LSB position in cell
    icell_t *getLSBCellPtr(uint8_t &r_bit_pos) noexcept;
    /// Returns position of cell with LSB and get LSB position in cell
    isize_t getLSBCellPos(uint8_t &r_bit_pos) const noexcept;

    /// Returns const pointer to cell with LSB and position of cell
    const icell_t *getLSBCellPtr(isize_t &r_pos) const noexcept;
    /// Returns pointer to cell with LSB and position of cell
    icell_t *getLSBCellPtr(isize_t &r_pos) noexcept;

    /// Returns LSB position in cell
    uint8_t getLSBPos() const noexcept;
    /// Returns LSB global position in cell  (could fail for large numbers)
    isize_t getLSBGlobalPos(isize_t *const overflow = nullptr) const noexcept; // handle edge case
    /// Returns LSB global position in cell
    InfiniterDerived getLSBGlobalPos() const; // throws bad_alloc


    InfiniterDerived &invert();
    static InfiniterDerived invert(const InfiniterDerived &p_value);


    inline ibit_t getBit(isize_t p_cell_index, uint8_t p_pos); // throws out_of_range
    inline void setBit(isize_t p_cell_index, uint8_t p_pos); // throws out_of_range
    inline void clearBit(isize_t p_cell_index, uint8_t p_pos); // throws out_of_range
    inline void toggleBit(isize_t p_cell_index, uint8_t p_pos); // throws out_of_range

    // unsafe or unchecked?
    inline ibit_t getBitUnsafe(uint64_t p_cell_index, uint8_t p_pos) noexcept;
    inline void setBitUnsafe(uint64_t p_cell_index, uint8_t p_pos) noexcept;
    inline void clearBitUnsafe(uint64_t p_cell_index, uint8_t p_pos) noexcept;
    inline void toggleBitUnsafe(uint64_t p_cell_index, uint8_t p_pos) noexcept;


    InfiniterDerived &shiftLSB(ibit_t p_lsb=IBIT_FALSE); // Least Significant Bit (<<)
    InfiniterDerived &shiftMSB(ibit_t p_msb=IBIT_FALSE); // Most Significant Bit (>>)

    inline InfiniterDerived &shiftLeft();
    inline InfiniterDerived &shiftRight();

    InfiniterDerived &shiftCellsLeft(isize_t p_cells);
    InfiniterDerived &shiftCellsRight(isize_t p_cells);

    inline InfiniterDerived &shiftLeft(isize_t p_operations);
    inline InfiniterDerived &shiftRight(isize_t p_operations);

    InfiniterDerived &pushLSB(ibit_t p_lsb);
    InfiniterDerived &pushMSB(ibit_t p_msb);


public:

    /// Operators
    inline InfiniterDerived operator ~ () const noexcept;


};

#include "InfiniterBit.tpp"
