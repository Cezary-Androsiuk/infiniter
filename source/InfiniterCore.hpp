#pragma once

#include "InfiniterShared.hpp"
#include "InfiniterMemory.hpp"

#include <cstdint>

template<typename InfiniterDerived>
class InfiniterCore : private InfiniterMemory
{
public:
    inline explicit InfiniterCore() noexcept;

    inline InfiniterCore(int64_t p_value); /// throws bad_alloc
    inline explicit InfiniterCore(int64_t p_value, isize_t p_capacity); /// throws bad_alloc
    inline explicit InfiniterCore(icell_t p_value, isize_t p_capacity, bool p_negative_value); /// throws bad_alloc
    inline explicit InfiniterCore(const icell_t *p_array, isize_t p_size, bool p_negative_value=false); /// throws bad_alloc

    inline InfiniterCore(const InfiniterDerived &p_source); /// throws bad_alloc
    inline InfiniterCore(InfiniterDerived &&p_source) noexcept;

    inline ~InfiniterCore() noexcept;

    inline InfiniterDerived &getRef() noexcept;
    inline const InfiniterDerived &getCRef() const noexcept;
    inline InfiniterDerived getCopy() const;

    InfiniterDerived &reset() noexcept;

protected:
    void clear() noexcept;
    void clearReserved() noexcept;

public:
    inline void reserve(isize_t p_new_capacity);
    inline void reserve(const InfiniterDerived &p_source);
    inline void extend(isize_t p_additional_capacity);

    inline void shrink();
    inline void optimize();

    void trim() noexcept; /// reduce size to trim not used zero-cells in front of the number
    inline InfiniterDerived &normalize() noexcept;

    inline icell_t *getData() noexcept;
    inline const icell_t *getData() const noexcept;

    inline isize_t getSize() const noexcept;
    inline isize_t getCapacity() const noexcept;
    inline ibit_t getSign() const noexcept;
    isize_t getRealSize() const noexcept;

    inline isize_t setSize(isize_t p_new_size) noexcept;
    isize_t setSizeWithExtend(isize_t p_new_size);

    inline InfiniterDerived &setSign(bool p_new_sign) noexcept;
    inline InfiniterDerived &setPositiveSign() noexcept;
    inline InfiniterDerived &setNegativeSign() noexcept;
    inline InfiniterDerived &negate() noexcept;

    inline InfiniterDerived &abs();
    static InfiniterDerived abs(const InfiniterDerived &p_value);

    InfiniterDerived &assign(icell_t p_value, bool p_negative_value=false) noexcept;
    InfiniterDerived &assign(const icell_t *p_array, isize_t p_size, bool p_negative_value=false);
    InfiniterDerived &assign(const InfiniterDerived &p_source);
    InfiniterDerived &assign(InfiniterDerived &&p_source);

protected:
    bool equalMagnitude(icell_t p_scalar) const noexcept;
    bool differsMagnitude(icell_t p_scalar) const noexcept;
    bool greaterMagnitude(icell_t p_scalar) const noexcept;
    bool smallerMagnitude(icell_t p_scalar) const noexcept;
    bool greaterEqualMagnitude(icell_t p_scalar) const noexcept;
    bool smallerEqualMagnitude(icell_t p_scalar) const noexcept;

    bool equalMagnitude(const InfiniterDerived &p_right) const noexcept;
    bool differsMagnitude(const InfiniterDerived &p_right) const noexcept;
    bool greaterMagnitude(const InfiniterDerived &p_right) const noexcept;
    bool smallerMagnitude(const InfiniterDerived &p_right) const noexcept;
    bool greaterEqualMagnitude(const InfiniterDerived &p_right) const noexcept;
    bool smallerEqualMagnitude(const InfiniterDerived &p_right) const noexcept;

public:
    bool equal(icell_t p_scalar, bool p_sign) const noexcept;
    bool differs(icell_t p_scalar, bool p_sign) const noexcept;
    bool greater(icell_t p_scalar, bool p_sign) const noexcept;
    bool smaller(icell_t p_scalar, bool p_sign) const noexcept;
    bool greaterEqual(icell_t p_scalar, bool p_sign) const noexcept;
    bool smallerEqual(icell_t p_scalar, bool p_sign) const noexcept;

    bool equal(const InfiniterDerived &p_right) const noexcept;
    bool differs(const InfiniterDerived &p_right) const noexcept;
    bool greater(const InfiniterDerived &p_right) const noexcept;
    bool smaller(const InfiniterDerived &p_right) const noexcept;
    bool greaterEqual(const InfiniterDerived &p_right) const noexcept;
    bool smallerEqual(const InfiniterDerived &p_right) const noexcept;

    bool is0() const noexcept;
    inline bool is1() const noexcept;
    inline bool is2() const noexcept;
    inline bool isPositive1() const noexcept;
    inline bool isNegative1() const noexcept;
    inline bool isPositive2() const noexcept;
    inline bool isNegative2() const noexcept;

    bool toBool() const noexcept;

    icell_t &getCell(isize_t p_cell_index);
    const icell_t &getCell(isize_t p_cell_index) const;
    inline icell_t &getCellUnsafe(isize_t p_cell_index) noexcept;
    inline const icell_t &getCellUnsafe(isize_t p_cell_index) const noexcept;

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
    InfiniterDerived &operator = (isval_t p_scalar);
    InfiniterDerived &operator = (const InfiniterDerived &p_right);
    InfiniterDerived &operator = (InfiniterDerived &&p_right);

    inline explicit operator bool () const noexcept;
    inline InfiniterDerived operator - () const noexcept;

    inline bool operator == (isval_t p_scalar) const noexcept;
    inline bool operator != (isval_t p_scalar) const noexcept;
    inline bool operator > (isval_t p_scalar) const noexcept;
    inline bool operator < (isval_t p_scalar) const noexcept;
    inline bool operator >= (isval_t p_scalar) const noexcept;
    inline bool operator <= (isval_t p_scalar) const noexcept;

    inline bool operator == (const InfiniterDerived &p_right) const noexcept;
    inline bool operator != (const InfiniterDerived &p_right) const noexcept;
    inline bool operator > (const InfiniterDerived &p_right) const noexcept;
    inline bool operator < (const InfiniterDerived &p_right) const noexcept;
    inline bool operator >= (const InfiniterDerived &p_right) const noexcept;
    inline bool operator <= (const InfiniterDerived &p_right) const noexcept;

    inline icell_t& operator [] (isize_t p_cell_index);
    inline const icell_t& operator [] (isize_t p_cell_index) const;

private:
    uint64_t m_size;

    friend void infiniterCoreTests();
};

#include "InfiniterCore.tpp"

