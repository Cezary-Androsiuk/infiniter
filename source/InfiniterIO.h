#ifndef INFINITERIO_H
#define INFINITERIO_H

#include "InfiniterCommon.hpp"
#include "InfiniterCore.h"

#include <string>

class InfiniterIO : public InfiniterCore
{
public:
    InfiniterIO();
    // constructors

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


    friend void infiniterIOTests();
};

#endif // INFINITERIO_H
