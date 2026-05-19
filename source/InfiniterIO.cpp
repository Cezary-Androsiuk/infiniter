#include "InfiniterIO.h"

#include "InfiniterCommon.hpp"

#include <utility> // std::move
#include <string>
#include <vector>


#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>



/// Constructed     TYPE    %p
/// Assigned        TYPE    %p
/// DEL                     %p
/// _iio_dbgprintf("IIO Constructed   DEFAULT       %p\n", this);
/// _iio_dbgprintf("IIO Constructed   PARAMETER     %p\n", this);
/// _iio_dbgprintf("IIO Constructed   COPY          %p\n", this);
/// _iio_dbgprintf("IIO Constructed   MOVE          %p\n", this);
/// _iio_dbgprintf("IIO DEL                         %p\n", this);
/// _iio_dbgprintf("IIO Assigned      COPY          %p\n", this);
/// _iio_dbgprintf("IIO Assigned      MOVE          %p\n", this);
#if INFINITER_IO_DEBUG_PRINT
#include <cstdio>
#define _iio_dbgprintf(...) printf(__VA_ARGS__);
#else // INFINITER_IO_DEBUG_PRINT
#define _iio_dbgprintf(...)
#endif // INFINITER_IO_DEBUG_PRINT


InfiniterIO::InfiniterIO() noexcept
    : InfiniterCore()
{
    _iio_dbgprintf("IIO Constructed   DEFAULT       %p\n", this);
}

InfiniterIO::InfiniterIO(uint64_t p_capacity)
    : InfiniterCore(p_capacity)
{
    _iio_dbgprintf("IIO Constructed   PARAMETER 1   %p\n", this);

}

InfiniterIO::InfiniterIO(uint64_t p_capacity, uint64_t p_value, bool p_negative_value)
    : InfiniterCore(p_capacity, p_value, p_negative_value)
{
    _iio_dbgprintf("IIO Constructed   PARAMETER 2   %p\n", this);

}

InfiniterIO::InfiniterIO(const cell_t *p_array, uint64_t p_size)
{
    _iio_dbgprintf("IIO Constructed   PARAMETER 3   %p\n", this);

}

InfiniterIO::InfiniterIO(const std::string &p_number)
{
    _iio_dbgprintf("IIO Constructed   PARAMETER 4   %p\n", this);

}

InfiniterIO::InfiniterIO(const std::vector &p_number)
{
    _iio_dbgprintf("IIO Constructed   PARAMETER 5   %p\n", this);

}

InfiniterIO::InfiniterIO(const InfiniterIO &p_source)
{
    _iio_dbgprintf("IIO Constructed   COPY          %p\n", this);

}

InfiniterIO::InfiniterIO(InfiniterIO &&p_source) noexcept
{
    _iio_dbgprintf("IIO Constructed   MOVE          %p\n", this);

}

InfiniterIO::~InfiniterIO() noexcept
{
    _iio_dbgprintf("IIO DEL                         %p\n", this);

    /// everything was done in InfiniterMemory
}

void InfiniterIO::serialize(std::string file_path) const
{
    FILE* file = fopen(file_path.c_str(), "wb");
    if(file == NULL)
    {
        int err = errno;
        fprintf(stderr, "Error: Cannot open \"%s\" file! Reason: %s\n", file_path.c_str(), strerror(err));
        return;
    }

    fwrite(this->getData(), sizeof(cell_t), this->getSize(), file);

    fclose(file);
}

void InfiniterIO::deserialize(std::string file_path)
{
    FILE* file = fopen(file_path.c_str(), "rb");
    if(file == NULL)
    {
        int err = errno;
        fprintf(stderr, "Error: Cannot open \"%s\" file! Reason: %s\n", file_path.c_str(), strerror(err));
        return;
    }

    /// check file size
    fseek(file, 0, SEEK_END);
    uint64_t fsize = ftell(file);
    rewind(file);

    /// check if data format is correct
    if(fsize % sizeof(cell_t) != 0 || fsize == 0)
    {
        fprintf(stderr, "Error: Invalid file format in \"%s\"!\n", file_path.c_str());
        fclose(file);
        return;
    }

    this->reset();
    this->reserve( fsize / sizeof(cell_t) );

    uint64_t bytes_read = fread(this->getData(), sizeof(cell_t), fsize, file);

    if (bytes_read != fsize) {
        fprintf(stderr, "Warning: Could not read the entire file properly, read %llu from %llu bytes.\n", bytes_read, fsize);
    }

    fclose(file);
}

void InfiniterIO::print(uint64_t base) const{
    if (base < 2) {
        std::cout << "Error: Base must be >= 2";
    }
    if (this->getSize() == 0 || (this->getSize() == 1 && this->getData()[0] == 0)) {
        std::cout << "0";
    }

    // Kopiujemy dane, żeby nie modyfikować oryginalnej liczby
    std::vector<uint64_t> temp(this->getData(), this->getData() + this->getSize());
    uint64_t current_size = this->getSize();
    std::string result = "";

    // Słownik dla standardowych systemów (do podstawy 36)
    const char* chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (current_size > 0) {
        uint64_t remainder = 0;

        // Dzielenie przez bazę, od najbardziej znaczącego bloku do najmniej
        for (int i = current_size - 1; i >= 0; --i) {
            // Sklejamy resztę z poprzedniego bloku (przesuniętą w lewo o 64 bity) z obecnym blokiem
            unsigned __int128 current = ((unsigned __int128)remainder << 64) | temp[i];

            temp[i] = (uint64_t)(current / base);
            remainder = (uint64_t)(current % base);
        }

        // Mapowanie wyliczonej reszty (cyfry) na znak
        if (base <= 36) {
            result += chars[remainder];
        } else {
            // Dla bardzo nietypowych, wielkich baz (np. base=100) brakuje znaków w alfabecie.
            // Zapisujemy wtedy wartość cyfry w nawiasach, np. [42][12]
            result += "[" + std::to_string(remainder) + "]";
        }

        // Usuwanie wiodących zer, aby zmniejszyć zakres operacji w następnej iteracji
        while (current_size > 0 && temp[current_size - 1] == 0) {
            current_size--;
        }
    }

    // Algorytm wylicza cyfry od najmłodszej, więc na koniec trzeba odwrócić string
    std::reverse(result.begin(), result.end());

    std::cout<< result;
}

InfiniterIO &InfiniterIO::operator =(const InfiniterIO &p_source)
{
    _iio_dbgprintf("IIO Assigned      COPY          %p\n", this);

}

InfiniterIO &InfiniterIO::operator =(InfiniterIO &&p_source)
{
    _iio_dbgprintf("IIO Assigned      MOVE          %p\n", this);

}



