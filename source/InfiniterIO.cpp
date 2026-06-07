#include "InfiniterIO.hpp"

#include "InfiniterShared.hpp"
#include "InfiniterException.hpp"

#include <utility> // std::move
#include <string>
#include <vector>

#include <cmath>
#include <memory> // std::unique_ptr


#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>



/// _io_dbgprintf("--- DEBUG IO %p | Constructed   DEFAULT\n", this);
/// _io_dbgprintf("--- DEBUG IO %p | Constructed   PARAMETER\n", this);
/// _io_dbgprintf("--- DEBUG IO %p | Constructed   COPY\n", this);
/// _io_dbgprintf("--- DEBUG IO %p | Constructed   MOVE\n", this);
/// _io_dbgprintf("--- DEBUG IO %p | Assigned      COPY\n", this);
/// _io_dbgprintf("--- DEBUG IO %p | Assigned      MOVE\n", this);
/// _io_dbgprintf("--- DEBUG IO %p | Delete\n", this);
/// _io_dbgprintf("--- DEBUG IO %p | Other         (...)\n", this);
#if IO_DEBUG_EXECUTION_PRINT
#include <cstdio>
#define _io_dbgprintf(...) printf(__VA_ARGS__);
#else // IO_DEBUG_EXECUTION_PRINT
#define _io_dbgprintf(...)
#endif // IO_DEBUG_EXECUTION_PRINT


InfiniterIO::InfiniterIO() noexcept
    : InfiniterUtility()
{
    _io_dbgprintf("--- DEBUG IO %p | Constructed   DEFAULT\n", this);
}

InfiniterIO::InfiniterIO(uint64_t p_capacity)
    : InfiniterUtility(p_capacity)
{
    _io_dbgprintf("--- DEBUG IO %p | Constructed   PARAMETER uint64_t\n", this);
}

InfiniterIO::InfiniterIO(uint64_t p_capacity, uint64_t p_value, bool p_negative_value)
    : InfiniterUtility(p_capacity, p_value, p_negative_value)
{
    _io_dbgprintf("--- DEBUG IO %p | Constructed   PARAMETER uint64_t uint64_t bool\n", this);
}

InfiniterIO::InfiniterIO(const cell_t *p_array, uint64_t p_size, bool p_negative_value)
    : InfiniterUtility(p_array, p_size, p_negative_value)
{
    _io_dbgprintf("--- DEBUG IO %p | Constructed   PARAMETER cell_t* uint64_t bool\n", this);
}

InfiniterIO::InfiniterIO(const std::string &p_number, int p_base, bool p_negative_value)
    : InfiniterUtility()
{
    _io_dbgprintf("--- DEBUG IO %p | Constructed   PARAMETER std::string int bool\n", this);

    if (p_base < 2 || p_base > 36)
    {
        throw InfiniterException::InvalidBase(p_base, 2, 36);
    }

    validateStringNumber(p_number, p_base);

    if(p_base == 2)
    {

    }

    size_t number_length = p_number.size();
    size_t binary_length = InfiniterIO::maxLengthAfterConversion(number_length, p_base, 2);

}

InfiniterIO::InfiniterIO(const std::vector<uint8_t> &p_number, int p_base, bool p_negative_value)
{
    _io_dbgprintf("--- DEBUG IO %p | Constructed   PARAMETER std::vector<uint8_t> int bool\n", this);
}

InfiniterIO::InfiniterIO(const std::vector<unsigned int> &p_number, int p_base, bool p_negative_value)
{
    _io_dbgprintf("--- DEBUG IO %p | Constructed   PARAMETER std::vector<unsigned int> int bool\n", this);
}

InfiniterIO::InfiniterIO(const std::vector<uint64_t> &p_number, int p_base, bool p_negative_value)
{
    _io_dbgprintf("--- DEBUG IO %p | Constructed   PARAMETER std::vector<uint64_t> int bool\n", this);
}

InfiniterIO::InfiniterIO(const InfiniterIO &p_source)
    : InfiniterUtility(p_source)
{
    _io_dbgprintf("--- DEBUG IO %p | Constructed   COPY\n", this);

}

InfiniterIO::InfiniterIO(InfiniterIO &&p_source) noexcept
    : InfiniterUtility(std::move(p_source))
{
    _io_dbgprintf("--- DEBUG IO %p | Constructed   MOVE\n", this);

}

InfiniterIO::~InfiniterIO() noexcept
{
    _io_dbgprintf("--- DEBUG IO %p | Delete\n", this);

    /// everything was done in InfiniterMemory
}

void InfiniterIO::serialize(std::string p_file_path, bool p_ignore_capacity) const
{
    FILE* file = fopen(p_file_path.c_str(), "wb");
    if(file == NULL)
    {
        int err = errno;
        fprintf(stderr, "Error: Cannot open \"%s\" file! Reason: %s\n", p_file_path.c_str(), strerror(err));
        return;
    }

    /// save header
    uint64_t size = this->getSize();
    uint64_t capacity = p_ignore_capacity ? size : this->getCapacity();
    uint64_t sign = static_cast<uint64_t>(this->getSign());
    /// to add more bit values divide size by half to uint32_t, uint16_t or uint8_t

    fwrite(&capacity, sizeof(uint64_t), 1, file);
    fwrite(&size, sizeof(uint64_t), 1, file);
    fwrite(&sign, sizeof(uint64_t), 1, file);

    /// save data
    /// m_size allways is greater than 0
    fwrite(this->getData(), sizeof(cell_t), this->getSize(), file);

    fclose(file);
}

void InfiniterIO::deserialize(std::string p_file_path, bool p_ignore_capacity)
{
    FILE* file = fopen(p_file_path.c_str(), "rb");
    if(file == NULL)
    {
        int err = errno;
        fprintf(stderr, "Error: Cannot open \"%s\" file! Reason: %s\n", p_file_path.c_str(), strerror(err));
        return;
    }

    /// check file size
    fseek(file, 0, SEEK_END);
    uint64_t fsize = ftell(file);
    rewind(file);

    /// check header
    constexpr uint64_t header_size = 3 * sizeof(uint64_t); /// 24 bytes

    if(fsize < header_size)
    {
        fprintf(stderr, "Error: File \"%s\" is too small to contain the header!\n", p_file_path.c_str());
        fclose(file);
        return;
    }

    /// read header
    uint64_t size = 1;
    uint64_t capacity = 1;
    uint64_t sign = 0;
    /// to add more bit values divide size by half to uint32_t, uint16_t or uint8_t

    fread(&capacity, sizeof(uint64_t), 1, file);
    fread(&size, sizeof(uint64_t), 1, file);
    fread(&sign, sizeof(uint64_t), 1, file);

    /// case protection
    if(capacity < size)
        capacity = size;

    /// check if data format is correct
    uint64_t expected_data_size = size * sizeof(cell_t);
    if(fsize - header_size != expected_data_size)
    {
        fprintf(stderr, "Error: Invalid file format in \"%s\"!\n", p_file_path.c_str());
        fclose(file);
        return;
    }

    this->reset();
    this->reserve( p_ignore_capacity ? size : capacity );
    this->setSize(size); /// ignore return value, size <= m_capacity
    this->setSign(sign);

    uint64_t bytes_read = fread(this->getData(), sizeof(cell_t), fsize, file);

    if (bytes_read != fsize) {
        fprintf(stderr, "Warning: Could not read the entire file properly, read %llu from %llu bytes.\n", bytes_read, fsize);
    }

    fclose(file);
}

bool InfiniterIO::isPowerOfTwo(int p_number)
{
    /// power of two has always one bit set to 1, and this formula checks that
    return (p_number > 0) && ((p_number & (p_number - 1)) == 0);
}

double InfiniterIO::log(double p_base, double p_value) noexcept
{
    if(p_base <= 1 || p_value <= 0)
        return 0.0;

    return std::log2(p_value) / std::log2(p_base);
}

uint64_t InfiniterIO::maxLengthAfterConversion(uint64_t p_number_length,
                                               uint8_t p_number_base,
                                               uint8_t p_target_base) noexcept
{
    if(p_number_base < 2 || p_target_base < 2)
        return 0;

    double ratio = InfiniterIO::log(p_number_base, p_target_base);

    return std::ceill(static_cast<double>(p_number_length) * ratio);
}

uint64_t InfiniterIO::estimateCellsByBase(uint64_t p_number_size, uint8_t p_base)
{
    const long double ratio = static_cast<long double>( InfiniterIO::log(2, p_base) );

    const long double estimated_result = std::ceill(
        static_cast<long double>(p_number_size) * ratio);

    constexpr long double UINT64_MAX_DBL = 0x1p64L; /// static_cast<long double>(UINT64_MAX);

    if(std::isinf(estimated_result) ||
        std::isnan(estimated_result) ||
        estimated_result > UINT64_MAX_DBL)
    {
        /// overflow occur
        throw InfiniterException::CapacityExceeded(
            "Number with length " + std::to_string(p_number_size) +
            " and base " + std::to_string(p_base) +
            " to binary conversion exceeds cell limit");
    }

    if(estimated_result <= 0.0L)
    {
        throw InfiniterException::CapacityApproximationFailed(
            "Number with length " + std::to_string(p_number_size) +
            " and base " + std::to_string(p_base) +
            " to binary conversion failed");
    }

    return static_cast<uint64_t>(estimated_result);
}

void InfiniterIO::validateStringNumber(const std::string &p_number, int p_base)
{
    /// limit base
    if (p_base < 2 || p_base > 36)
    {
        throw InfiniterException::InvalidBase(p_base, 2, 36);
    }

    /// empty string
    if (p_number.empty())
    {
        throw InfiniterException::InvalidStringFormat("Empty string");
    }

    /// handle leading zeros (01 or 001123 are not allowed)
    if (p_number[0] == '0') {
        throw InfiniterException::InvalidStringFormat("Leading zeros are not allowed");
    }

    /// main validation
    for (size_t i = 0; i < p_number.length(); ++i) {
        const char c = p_number[i];
        bool is_valid = false;

        if (c >= '0' && c <= '9') {
            if (c - '0' < p_base) is_valid = true;
        }
        else if (c >= 'a' && c <= 'z') {
            if (10 + (c - 'a') < p_base) is_valid = true;
        }
        else if (c >= 'A' && c <= 'Z') {
            if (10 + (c - 'A') < p_base) is_valid = true;
        }

        if (!is_valid) {
            throw InfiniterException::InvalidStringFormat(
                std::string("String contains invalid character '") + c +
                "' for base " + std::to_string(p_base)
                );
        }
    }
}

void InfiniterIO::assignStringBase2(const std::string &p_binary_number)
{
    /// compute required cells
    uint64_t requiredCells = p_binary_number.size() / BITS_PER_CELL;
    if(p_binary_number.size() % BITS_PER_CELL)
        requiredCells++;

    /// prepare data
    this->reserve(requiredCells);
    this->setSize(requiredCells);
    cell_t *data = this->getData();
    cell_t *data_ptr = data;
    const char *raw_number = p_binary_number.data();

    size_t size = p_binary_number.size();
    while(size != 0)
    {
        size_t part = std::min(static_cast<size_t>(BITS_PER_CELL), size);
        cell_t tmp_cell = 0;

        /// for each bit in cell emplace bit from string
        for(size_t i=0; i<part; i++)
        {
            const size_t i_rev = size -1 -i;
            const cell_t bit_value = (raw_number[i_rev] == '0') ? 0 : 1;

            tmp_cell <<= 1;
            tmp_cell |= bit_value;
        }
        size -= part;

        /// emplace cell in data array
        *data_ptr = tmp_cell;
        data_ptr++;
    }
}

void InfiniterIO::assignString(const std::string &p_number, uint8_t p_base)
{
    /// max base is 36 (needs ~5.16 times more characters to write it in binary)
    /// if
    if(p_number.size() > UINT64_MAX / (64*6))
    {
        // printf("Warning: ")
        UINT32_MAX;
    }

    uint64_t binary_cells_total = estimateCellsByBase(p_number.size(), p_base);



    // /// compute required cells
    // uint64_t requiredCells = p_binary_number.size() / BITS_PER_CELL;
    // if(p_binary_number.size() % BITS_PER_CELL)
    //     requiredCells++;

    // size_t number_length = p_number.size();
    // size_t binary_length = InfiniterIO::maxLengthAfterConversion(number_length, p_base, 2);

    // /// prepare data
    // this->reserve(requiredCells);
    // this->setSize(requiredCells);
    // cell_t *data = this->getData();
    // cell_t *data_ptr = data;
    // const char *raw_number = p_binary_number.data();

    // size_t size = p_binary_number.size();
    // while(size != 0)
    // {
    //     size_t part = std::min(static_cast<size_t>(BITS_PER_CELL), size);
    //     cell_t tmp_cell = 0;

    //     /// for each bit in cell emplace bit from string
    //     for(size_t i=0; i<part; i++)
    //     {
    //         const size_t i_rev = size -1 -i;
    //         const cell_t bit_value = (raw_number[i_rev] == '0') ? 0 : 1;

    //         tmp_cell <<= 1;
    //         tmp_cell |= bit_value;
    //     }
    //     size -= part;

    //     /// emplace cell in data array
    //     *data_ptr = tmp_cell;
    //     data_ptr++;
    // }
}


void InfiniterIO::assign(const std::string &p_number, uint8_t p_base, bool p_negative_value)
{
    if(InfiniterIO::isPowerOfTwo(p_base))
    {
        this->assignStringBase2(p_number);
    }
    else
    {

    }

    this->setSign(p_negative_value);
}

// void InfiniterIO::print(uint64_t p_base) const
// {
//     if(p_base < 2 || 16  < p_base)
//         printf("No valid base (2-16)\n");

//     switch(p_base)
//     {
//     case 2:     this->printBase2();     return;
//     case 4:     this->printBase4();     return;
//     case 8:     this->printBase8();     return;
//     case 16:    this->printBase16();    return;
//     }


// }

void InfiniterIO::printBase2() const
{
    const cell_t *data = this->getData();
    const uint64_t size = this->getSize();

    char cell_buffer[64+1];

    bool non_zero_cell_found = false;

    for(uint64_t i=0; i<size; i++)
    {
        cell_t cell = data[size -i -1];
        if(cell == 0 && !non_zero_cell_found)
            continue;

        non_zero_cell_found = true;

        for(int i=0; i<64; i++)
        {
            bool bit = cell & (1ull << (63-i));
            cell_buffer[i] = bit ? '1' : '0';
        }
        cell_buffer[64] = 0;

        if(i+1 == size)
            printf("%s", cell_buffer);
        else
            printf("%s ", cell_buffer);
    }

    if(!non_zero_cell_found)
        printf("0");
}

void InfiniterIO::printBase4() const
{

}

void InfiniterIO::printBase8() const
{

}

void InfiniterIO::printBase16() const
{

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
    _io_dbgprintf("--- DEBUG IO %p | Assigned      COPY\n", this);

}

InfiniterIO &InfiniterIO::operator =(InfiniterIO &&p_source)
{
    _io_dbgprintf("--- DEBUG IO %p | Assigned      MOVE\n", this);

}



