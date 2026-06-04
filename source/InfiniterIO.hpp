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

    explicit InfiniterIO(uint64_t p_capacity); /// throws bad_alloc
    explicit InfiniterIO(uint64_t p_capacity, uint64_t p_value, bool p_negative_value=false); /// throws bad_alloc
    explicit InfiniterIO(const cell_t *p_array, uint64_t p_size, bool p_negative_value=false); /// throws bad_alloc

    explicit InfiniterIO(const std::string &p_number, int p_base=10, bool p_negative_value=false); /// throws bad_alloc
    explicit InfiniterIO(const std::vector<uint8_t> &p_number, int p_base=10, bool p_negative_value=false); /// throws bad_alloc
    explicit InfiniterIO(const std::vector<unsigned int> &p_number, int p_base=10, bool p_negative_value=false); /// throws bad_alloc
    explicit InfiniterIO(const std::vector<uint64_t> &p_number, int p_base=10, bool p_negative_value=false); /// throws bad_alloc

    explicit InfiniterIO(const InfiniterIO &p_source); /// throws bad_alloc
    explicit InfiniterIO(InfiniterIO &&p_source) noexcept;

    ~InfiniterIO() noexcept;

    void serialize(std::string p_file_path, bool p_ignore_capacity = false) const;
    void deserialize(std::string p_file_path, bool p_ignore_capacity = false);

protected:
    static bool isPowerOfTwo(int p_number);
public:

    void assign(uint64_t p_value, bool p_negative_value=false) noexcept;
    void assign(const cell_t *p_array, uint64_t p_size, bool p_negative_value=false);

    void assign(const std::string &p_number, int p_base=10, bool p_negative_value=false);

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
#if IIO_ENABLE_DB_PRINT_METHOD
#define iio_dbg_print(obj) printf("IIO "); static_cast<InfiniterCore>(obj).dbg_print()
#else // IIO_ENABLE_DB_PRINT_METHOD
#define iio_dbg_print(obj) printf("iio_dbg_print disabled\n");
#endif // IIO_ENABLE_DB_PRINT_METHOD
public:


    /// Operators
    InfiniterIO &operator =(const InfiniterIO &p_source);
    InfiniterIO &operator =(InfiniterIO &&p_source);


    friend void infiniterIOTests();
};

#endif // INFINITERIO_HPP
