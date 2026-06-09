#ifndef INFINITERIO_HPP
#define INFINITERIO_HPP

#include "InfiniterShared.hpp"
#include "InfiniterArithmetic.hpp"

#include <string>
#include <vector>

class InfiniterIO : public InfiniterArithmetic
{
public:
    explicit InfiniterIO() noexcept;

    explicit InfiniterIO(isize_t p_capacity); /// throws bad_alloc
    explicit InfiniterIO(isize_t p_capacity, icell_t p_value, bool p_negative_value=false); /// throws bad_alloc
    explicit InfiniterIO(const icell_t *p_array, isize_t p_size, bool p_negative_value=false); /// throws bad_alloc

    explicit InfiniterIO(const std::string &p_number, int p_base=10, bool p_negative_value=false); /// throws bad_alloc /// InfiniterException::InvalidStringFormat
    explicit InfiniterIO(const std::vector<uint8_t> &p_number, int p_base=10, bool p_negative_value=false); /// throws bad_alloc

    InfiniterIO(const InfiniterIO &p_source); /// throws bad_alloc
    InfiniterIO(InfiniterIO &&p_source) noexcept;

    InfiniterIO(const InfiniterArithmetic &p_source); /// throws bad_alloc
    InfiniterIO(InfiniterArithmetic &&p_source) noexcept;

    InfiniterIO(const InfiniterBit &p_source); /// throws bad_alloc
    InfiniterIO(InfiniterBit &&p_source) noexcept;

    InfiniterIO(const InfiniterCore &p_source); /// throws bad_alloc
    InfiniterIO(InfiniterCore &&p_source) noexcept;

    ~InfiniterIO() noexcept;

    /// InfiniterArithmetic methods

    void serialize(std::string p_file_path, bool p_ignore_capacity = false) const;
    void deserialize(std::string p_file_path, bool p_ignore_capacity = false);

protected:
    static bool isPowerOfTwo(int p_number);
    static double log(double p_base, double p_value) noexcept;

    static uint64_t estimateBitsByBase(uint64_t p_number_size, uint8_t p_base);
    static uint64_t estimateCellsByBase(uint64_t p_number_size, uint8_t p_base);

    static void validateStringNumber(const std::string &p_number, int p_base);
    static void validateVectorNumber(const std::vector<uint8_t> &p_number, int p_base);

public:

protected:
    void assignStringBase2(const std::string &p_binary_number);
    void assignString(std::string p_number, uint8_t p_base);

    void assignVectorBase2(const std::vector<uint8_t> &p_binary_number);
    void assignVector(std::vector<uint8_t> p_number, uint8_t p_base);

public:

    using InfiniterArithmetic::assign; /// prevent shadowing
    void assign(const std::string &p_number, uint8_t p_base=10, bool p_negative_value=false);
    void assign(const std::vector<uint8_t> &p_number, uint8_t p_base=10, bool p_negative_value=false);
    void assign(const InfiniterIO &p_source);
    void assign(InfiniterIO &&p_source);

protected:
    std::string toStringBase2() const;
    std::vector<uint8_t> toVectorBase2() const;

public:

    std::string toString(uint8_t p_base) const;
    std::vector<uint8_t> toVector(uint8_t p_base) const;

    void print(uint64_t base) const;

protected:
    void printBase2() const;
    void printBase4() const;
    void printBase8() const;
    void printBase16() const;

    void saveToFile(); //////


public:
#if IO_ENABLE_DB_PRINT_METHOD
#define io_dbg_print(obj) printf("IIO "); static_cast<InfiniterCore>(obj).dbg_print()
#else // IO_ENABLE_DB_PRINT_METHOD
#define io_dbg_print(obj) printf("io_dbg_print disabled\n");
#endif // IO_ENABLE_DB_PRINT_METHOD
public:


    /// Operators
    using InfiniterArithmetic::operator=;
    InfiniterIO &operator =(const InfiniterIO &p_source);
    InfiniterIO &operator =(InfiniterIO &&p_source);


    friend void infiniterIOTests();
};

#endif // INFINITERIO_HPP
