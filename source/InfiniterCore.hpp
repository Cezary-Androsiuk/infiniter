#ifndef INFINITERCORE_HPP
#define INFINITERCORE_HPP

#include "InfiniterShared.hpp"
#include "InfiniterMemory.hpp"

#include <cstdint>

class InfiniterCore : private InfiniterMemory
{
public:
    explicit InfiniterCore() noexcept;

    explicit InfiniterCore(isize_t p_capacity); /// throws bad_alloc
    /// value only covers first cell, purpose of this is to initialize instance with 1 or other scalar values
    explicit InfiniterCore(isize_t p_capacity, icell_t p_value, bool p_negative_value=false); /// throws bad_alloc
    explicit InfiniterCore(const icell_t *p_array, isize_t p_size, bool p_negative_value=false); /// throws bad_alloc

    InfiniterCore(const InfiniterCore &p_source); /// throws bad_alloc
    InfiniterCore(InfiniterCore &&p_source) noexcept;

    ~InfiniterCore() noexcept;

    void reset() noexcept;

protected:
    void clear() noexcept;
    void clearReserved() noexcept;

    isize_t realSize() const noexcept;

public:
    void reserve(isize_t p_new_capacity);
    void reserve(const InfiniterCore &p_source);
    void extend(isize_t p_additional_capacity);

    void shrink();
    void optimize();

    /**
     * @brief trim
     * reduce size to trim not used zero-cells in front of the number
     */
    void trim();

    icell_t *getData() noexcept;
    const icell_t *getData() const noexcept;

    isize_t getSize() const noexcept;
    isize_t getCapacity() const noexcept;
    ibit_t getSign() const noexcept;

    /// returns what size was set
    /// if p_new_size > m_capacity, then size will be set to m_capacity
    isize_t setSize(isize_t p_new_size) noexcept;
    isize_t setSizeWithExtend(isize_t p_new_size);

    void setSign(bool p_new_sign) noexcept;
    void setPositiveSign() noexcept;
    void setNegativeSign() noexcept;

    void assign(icell_t p_value, bool p_negative_value=false) noexcept;
    void assign(const icell_t *p_array, isize_t p_size, bool p_negative_value=false);
    void assign(const InfiniterCore &p_source);
    void assign(InfiniterCore &&p_source);

    bool toBool() const noexcept;
    bool equal(const InfiniterCore &p_source) const noexcept;
    bool differs(const InfiniterCore &p_source) const noexcept;
    bool greater(const InfiniterCore &p_source) const noexcept;
    bool smaller(const InfiniterCore &p_source) const noexcept;
    bool greaterEqual(const InfiniterCore &p_source) const noexcept;
    bool smallerEqual(const InfiniterCore &p_source) const noexcept;

    icell_t &getCell(isize_t p_cell_index);
    const icell_t &getCell(isize_t p_cell_index) const;

public:
#if IC_ENABLE_DB_PRINT_METHOD

    void dbg_print_data() const;
#define ic_dbg_print_data(obj) obj.dbg_print_data() /// macro for safe usage
    void dbg_print_memory() const;
#define ic_dbg_print_memory(obj) obj.dbg_print_memory() /// macro for safe usage

#else // IC_ENABLE_DB_PRINT_METHOD

#define ic_dbg_print_data(obj) printf("ic_dbg_print_memory disabled\n") /// macro for safe usage
#define ic_dbg_print_memory(obj) printf("ic_dbg_print_memory disabled\n") /// macro for safe usage

#endif // IC_ENABLE_DB_PRINT_METHOD
public:

    /// Operators
    InfiniterCore &operator = (const InfiniterCore &p_source);
    InfiniterCore &operator = (InfiniterCore &&p_source);

    explicit operator bool () const noexcept;
    bool operator == (const InfiniterCore &p_source) const noexcept;
    bool operator != (const InfiniterCore &p_source) const noexcept;
    bool operator > (const InfiniterCore &p_source) const noexcept;
    bool operator < (const InfiniterCore &p_source) const noexcept;
    bool operator >= (const InfiniterCore &p_source) const noexcept;
    bool operator <= (const InfiniterCore &p_source) const noexcept;

    icell_t& operator [] (isize_t p_cell_index);
    const icell_t& operator [] (isize_t p_cell_index) const;

private:
    uint64_t m_size;

    friend void infiniterCoreTests();
};

#endif // INFINITERCORE_HPP
