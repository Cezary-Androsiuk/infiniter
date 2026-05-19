#ifndef INFINITERIO_H
#define INFINITERIO_H

#include "InfiniterCommon.hpp"
#include "InfiniterCore.h"

// #include <string>
// #include <vector>

namespace std{
class string;
class vector;
}

class InfiniterIO : public InfiniterCore
{
public:
    explicit InfiniterIO() noexcept;

    explicit InfiniterIO(uint64_t p_capacity); /// throws bad_alloc
    explicit InfiniterIO(uint64_t p_capacity, uint64_t p_value, bool p_negative_value=false); /// throws bad_alloc
    explicit InfiniterIO(const cell_t *p_array, uint64_t p_size);

    explicit InfiniterIO(const std::string &p_number); /// throws bad_alloc
    explicit InfiniterIO(const std::vector &p_number); /// throws bad_alloc

    explicit InfiniterIO(const InfiniterIO &p_source); /// throws bad_alloc
    explicit InfiniterIO(InfiniterIO &&p_source) noexcept;

    ~InfiniterIO() noexcept;

    void serialize(std::string file_path) const;
    void deserialize(std::string file_path);

    void print(uint64_t base) const;

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

#endif // INFINITERIO_H
