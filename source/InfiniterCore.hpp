#ifndef INFINITERCORE_HPP
#define INFINITERCORE_HPP

#include "InfiniterShared.hpp"
#include "InfiniterMemory.hpp"

#include <cstdint>

class InfiniterCore : private InfiniterMemory
{
public:
    explicit InfiniterCore() noexcept;

    explicit InfiniterCore(uint64_t p_capacity); /// throws bad_alloc
    /// value only covers first cell, purpose of this is to initialize instance with 1 or other scalar values
    explicit InfiniterCore(uint64_t p_capacity, uint64_t p_value, bool p_negative_value=false); /// throws bad_alloc
    explicit InfiniterCore(const cell_t *p_array, uint64_t p_size, bool p_negative_value=false); /// throws bad_alloc

    explicit InfiniterCore(const InfiniterCore &p_source); /// throws bad_alloc
    explicit InfiniterCore(InfiniterCore &&p_source) noexcept;

    ~InfiniterCore() noexcept;

    void reset() noexcept;

protected:
    void clear() noexcept;
    void clearReserved() noexcept;

public:
    void reserve(uint64_t p_new_capacity);
    void reserve(const InfiniterCore &p_source);
    void extend(uint64_t p_additional_capacity);

    void shrink();
    void optimize();

    /**
     * @brief trim
     * reduce size to trim not used zero-cells in front of the number
     */
    void trim();

    cell_t *getData() noexcept;
    const cell_t *getData() const noexcept;

    uint64_t getSize() const noexcept;
    uint64_t getCapacity() const noexcept;
    uint8_t getSign() const noexcept;

    /// returns what size was set
    /// if p_new_size > m_capacity, then size will be set to m_capacity
    uint64_t setSize(uint64_t p_new_size) noexcept;
    uint64_t setSizeWithExtend(uint64_t p_new_size);

    void setSign(bool p_new_sign) noexcept;
    void setPositiveSign() noexcept;
    void setNegativeSign() noexcept;

    void assign(uint64_t p_value, bool p_negative_value=false) noexcept;
    void assign(const cell_t *p_array, uint64_t p_size, bool p_negative_value=false);


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
    InfiniterCore &operator =(const InfiniterCore &p_source);
    InfiniterCore &operator =(InfiniterCore &&p_source);

private:
    uint64_t m_size;

    friend void infiniterCoreTests();
};

#endif // INFINITERCORE_HPP
