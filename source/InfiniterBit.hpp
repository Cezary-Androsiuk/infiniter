#ifndef INFINITERBIT_HPP
#define INFINITERBIT_HPP

#include "InfiniterShared.hpp"
#include "InfiniterCore.hpp"

class Infiniter;

class InfiniterBit : public InfiniterCore
{
public:
    explicit InfiniterBit() noexcept;

    explicit InfiniterBit(isize_t p_capacity); /// throws bad_alloc
    explicit InfiniterBit(isize_t p_capacity, icell_t p_value, bool p_negative_value=false); /// throws bad_alloc
    explicit InfiniterBit(const icell_t *p_array, isize_t p_size, bool p_negative_value=false); /// throws bad_alloc

    explicit InfiniterBit(const InfiniterBit &p_source); /// throws bad_alloc
    explicit InfiniterBit(InfiniterBit &&p_source) noexcept;

    ~InfiniterBit() noexcept;

    /// InfiniterCore methods

    // maybe change all uint64_t to cell where context is abaout bits or cells

    bool checkCellPos(isize_t cell_id, uint8_t pos) const;
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

    /// Returns MSB position in cell
    uint8_t getMSBPos() const noexcept;/////////////////////////////////////////////////// done here
    /// Returns MSB global position in cell  (could fail for large numbers)
    isize_t getMSBGlobalPosUnsafe() const noexcept; // handle edge case
    /// Returns MSB global position in cell
    Infiniter getMSBGlobalPos() const; // throws bad_alloc

    /// Returns MSB position in cell and get MSB cell value
    uint8_t getMSBPos(icell_t &r_cell) const noexcept;
    /// Returns MSB position in cell and get MSB cell pointer
    uint8_t getMSBPos(icell_t *&r_cell_ptr) const noexcept;
    /// Returns MSB position in cell and get MSB cell position
    uint8_t getMSBPosCellPos(isize_t &r_cell_pos) const noexcept;



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

    /// Returns LSB position in cell
    uint8_t getLSBPos() const noexcept;
    /// Returns LSB global position in cell  (could fail for large numbers)
    isize_t getLSBGlobalPosUnsafe() const noexcept; // handle edge case
    /// Returns LSB global position in cell
    Infiniter getLSBGlobalPos() const; // throws bad_alloc

    /// Returns LSB position in cell and get LSB cell value
    uint8_t getLSBPos(icell_t &r_cell) const noexcept;
    /// Returns LSB position in cell and get LSB cell pointer
    uint8_t getLSBPos(icell_t *&r_cell_ptr) const noexcept;
    /// Returns LSB position in cell and get LSB cell position
    uint8_t getLSBPosCellPos(isize_t &r_cell_pos) const noexcept;



    ibit_t getBit(isize_t p_cell_index, uint8_t p_pos); // throws out_of_range
    void setBit(isize_t p_cell_index, uint8_t p_pos); // throws out_of_range
    void clearBit(isize_t p_cell_index, uint8_t p_pos); // throws out_of_range
    void toggleBit(isize_t p_cell_index, uint8_t p_pos); // throws out_of_range

    // unsafe or unchecked?
    ibit_t getBitUnsafe(uint64_t p_cell_index, uint8_t p_pos) noexcept;
    void setBitUnsafe(uint64_t p_cell_index, uint8_t p_pos) noexcept;
    void clearBitUnsafe(uint64_t p_cell_index, uint8_t p_pos) noexcept;
    void toggleBitUnsafe(uint64_t p_cell_index, uint8_t p_pos) noexcept;


    void shiftLSB(ibit_t p_lsb=IBIT_FALSE); // Least Significant Bit (<<)
    void shiftMSB(ibit_t p_msb=IBIT_FALSE); // Most Significant Bit (>>)

    void shiftLeft();
    void shiftRight();

    void pushLSB(ibit_t p_lsb);
    void pushMSB(ibit_t p_msb); /// not finished - make it extend if needed
    

public:

    /// Operators
    using InfiniterCore::operator=;
    InfiniterBit &operator =(const InfiniterBit &p_source);
    InfiniterBit &operator =(InfiniterBit &&p_source);


};

#endif // INFINITERBIT_HPP
