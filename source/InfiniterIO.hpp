#ifndef INFINITERIO_HPP
#define INFINITERIO_HPP

#include "InfiniterShared.hpp"
#include "InfiniterUtility.hpp"

#include <string>
#include <vector>

class InfiniterIO : public InfiniterUtility
{
public:
    explicit InfiniterIO() noexcept;

    explicit InfiniterIO(isize_t p_capacity); /// throws bad_alloc
    explicit InfiniterIO(isize_t p_capacity, icell_t p_value, bool p_negative_value=false); /// throws bad_alloc
    explicit InfiniterIO(const icell_t *p_array, isize_t p_size, bool p_negative_value=false); /// throws bad_alloc

    explicit InfiniterIO(const std::string &p_number, int p_base=10, bool p_negative_value=false); /// throws bad_alloc /// InfiniterException::InvalidStringFormat
    explicit InfiniterIO(const std::vector<uint8_t> &p_number, int p_base=10, bool p_negative_value=false); /// throws bad_alloc
    explicit InfiniterIO(const std::vector<unsigned int> &p_number, int p_base=10, bool p_negative_value=false); /// throws bad_alloc
    explicit InfiniterIO(const std::vector<uint64_t> &p_number, int p_base=10, bool p_negative_value=false); /// throws bad_alloc

    explicit InfiniterIO(const InfiniterIO &p_source); /// throws bad_alloc
    explicit InfiniterIO(InfiniterIO &&p_source) noexcept;

    ~InfiniterIO() noexcept;

    /// InfiniterUtility methods

    void serialize(std::string p_file_path, bool p_ignore_capacity = false) const;
    void deserialize(std::string p_file_path, bool p_ignore_capacity = false);

protected:
    static bool isPowerOfTwo(int p_number);
    static double log(double p_base, double p_value) noexcept;
    static uint64_t maxLengthAfterConversion(uint64_t p_number_length,
                                             uint8_t p_number_base,
                                             uint8_t p_target_base) noexcept;
    static uint64_t estimateCellsByBase(uint64_t p_number_size, uint8_t p_base); // throws InfiniterException::CapacityExceeded and CapacityApproximationFailed
    static void validateStringNumber(const std::string &p_number, int p_base);
public:

protected:
    void assignStringBase2(const std::string &p_binary_number);
    void assignString(const std::string &p_number, uint8_t p_base); // throws InfiniterException::CapacityExceeded and CapacityApproximationFailed
public:


    using InfiniterCore::assign; /// prevent shadowing
    void assign(const std::string &p_number, uint8_t p_base=10, bool p_negative_value=false); // throws InfiniterException::CapacityExceeded and CapacityApproximationFailed

    void print(uint64_t base) const;

protected:
    void printBase2() const;
    void printBase4() const;
    void printBase8() const;
    void printBase16() const;

    void saveToFile(); //////

private:
    void assign_base64();

public:
#if IO_ENABLE_DB_PRINT_METHOD
#define io_dbg_print(obj) printf("IIO "); static_cast<InfiniterCore>(obj).dbg_print()
#else // IO_ENABLE_DB_PRINT_METHOD
#define io_dbg_print(obj) printf("io_dbg_print disabled\n");
#endif // IO_ENABLE_DB_PRINT_METHOD
public:


    /// Operators
    InfiniterIO &operator =(const InfiniterIO &p_source);
    InfiniterIO &operator =(InfiniterIO &&p_source);


    friend void infiniterIOTests();
};

#endif // INFINITERIO_HPP
