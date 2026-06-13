#pragma once

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


template<typename InfiniterDerived>
InfiniterIO<InfiniterDerived>::InfiniterIO() noexcept
    : InfiniterArithmetic<InfiniterDerived>()
{
    _io_dbgprintf("--- DEBUG IO %p | Constructed   DEFAULT\n", this);
}

template<typename InfiniterDerived>
InfiniterIO<InfiniterDerived>::InfiniterIO(isize_t p_capacity)
    : InfiniterArithmetic<InfiniterDerived>(p_capacity)
{
    _io_dbgprintf("--- DEBUG IO %p | Constructed   PARAMETER isize_t\n", this);
}

template<typename InfiniterDerived>
InfiniterIO<InfiniterDerived>::InfiniterIO(isize_t p_capacity, icell_t p_value, bool p_negative_value)
    : InfiniterArithmetic<InfiniterDerived>(p_capacity, p_value, p_negative_value)
{
    _io_dbgprintf("--- DEBUG IO %p | Constructed   PARAMETER isize_t icell_t bool\n", this);
}

template<typename InfiniterDerived>
InfiniterIO<InfiniterDerived>::InfiniterIO(const icell_t *p_array, isize_t p_size, bool p_negative_value)
    : InfiniterArithmetic<InfiniterDerived>(p_array, p_size, p_negative_value)
{
    _io_dbgprintf("--- DEBUG IO %p | Constructed   PARAMETER icell_t* isize_t bool\n", this);
}

template<typename InfiniterDerived>
InfiniterIO<InfiniterDerived>::InfiniterIO(const std::string &p_number, int p_base, bool p_negative_value)
    : InfiniterArithmetic<InfiniterDerived>()
{
    _io_dbgprintf("--- DEBUG IO %p | Constructed   PARAMETER std::string int bool\n", this);

    this->assign(p_number, p_base, p_negative_value);
}

template<typename InfiniterDerived>
InfiniterIO<InfiniterDerived>::InfiniterIO(const std::vector<uint8_t> &p_number, int p_base, bool p_negative_value)
    : InfiniterArithmetic<InfiniterDerived>()
{
    _io_dbgprintf("--- DEBUG IO %p | Constructed   PARAMETER std::vector<uint8_t> int bool\n", this);

    this->assign(p_number, p_base, p_negative_value);
}

template<typename InfiniterDerived>
InfiniterIO<InfiniterDerived>::InfiniterIO(const InfiniterDerived &p_source)
    : InfiniterArithmetic<InfiniterDerived>(p_source)
{
    _io_dbgprintf("--- DEBUG IO %p | Constructed IO COPY\n", this);
}

template<typename InfiniterDerived>
InfiniterIO<InfiniterDerived>::InfiniterIO(InfiniterDerived &&p_source) noexcept
    : InfiniterArithmetic<InfiniterDerived>(std::move(p_source))
{
    _io_dbgprintf("--- DEBUG IO %p | Constructed IO MOVE\n", this);
}

template<typename InfiniterDerived>
InfiniterIO<InfiniterDerived>::~InfiniterIO() noexcept
{
    _io_dbgprintf("--- DEBUG IO %p | Delete\n", this);

    /// everything was done in InfiniterMemory
}

template<typename InfiniterDerived>
void InfiniterIO<InfiniterDerived>::serialize(std::string p_file_path, bool p_ignore_capacity) const
{
    FILE* file = fopen(p_file_path.c_str(), "wb");
    if(file == NULL)
    {
        int err = errno;
        fprintf(stderr, "Error: Cannot open \"%s\" file! Reason: %s\n", p_file_path.c_str(), strerror(err));
        return;
    }

    /// save header
    uint64_t size = static_cast<uint64_t>(this->getSize());
    uint64_t capacity = static_cast<uint64_t>(p_ignore_capacity ? size : this->getCapacity());
    uint64_t sign = static_cast<uint64_t>(this->getSign());
    /// to add more bit values divide size by half to uint32_t, uint16_t or uint8_t

    fwrite(&capacity, sizeof(uint64_t), 1, file);
    fwrite(&size, sizeof(uint64_t), 1, file);
    fwrite(&sign, sizeof(uint64_t), 1, file);

    /// save data
    /// m_size allways is greater than 0
    fwrite(this->getData(), sizeof(icell_t), this->getSize(), file);

    fclose(file);
}

template<typename InfiniterDerived>
void InfiniterIO<InfiniterDerived>::deserialize(std::string p_file_path, bool p_ignore_capacity)
{
    FILE* file = fopen(p_file_path.c_str(), "rb");
    if(file == NULL)
    {
        int err = errno;
        fprintf(stderr, "Error: Cannot open \"%s\" file! "
                        "Reason: %s\n", p_file_path.c_str(), strerror(err));
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
        fprintf(stderr, "Error: File \"%s\" is too small to "
                        "contain the header!\n", p_file_path.c_str());
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
    uint64_t expected_data_size = size * sizeof(icell_t);
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

    uint64_t bytes_read = fread(this->getData(), sizeof(icell_t), size, file);

    if (bytes_read != size) {
        fprintf(stderr, "Warning: Could not read the entire file properly, "
                        "read %llu from %llu bytes.\n", bytes_read, size);
    }

    fclose(file);
}

template<typename InfiniterDerived>
bool InfiniterIO<InfiniterDerived>::isPowerOfTwo(int p_number)
{
    /// power of two has always one bit set to 1, and this formula checks that
    return (p_number > 0) && ((p_number & (p_number - 1)) == 0);
}

template<typename InfiniterDerived>
double InfiniterIO<InfiniterDerived>::log(double p_base, double p_value) noexcept
{
    return (p_base <= 1.0 || p_value <= 0.0) ? 0.0 : (std::log2(p_value) / std::log2(p_base));
}

template<typename InfiniterDerived>
uint64_t InfiniterIO<InfiniterDerived>::estimateBitsByBase(uint64_t p_number_size, uint8_t p_base)
{
    /// limit number_size
    if(p_number_size > IO_MAX_CONTAINER_INPUT_CAPACITY)
    {
        throw InfiniterException::InputContainerCapacityExceeded(
            p_number_size, IO_MAX_CONTAINER_INPUT_CAPACITY);
    }

    /// limit base
    if (p_base < 2 || p_base > 36)
    {
        throw InfiniterException::InvalidBase(p_base, 2, 36);
    }


    const long double ratio = static_cast<long double>( InfiniterIO::log(2, p_base) );

    const long double estimated_bits = std::ceill(
        static_cast<long double>(p_number_size) * ratio);

    if(estimated_bits <= 0.0L)
    {
        throw InfiniterException::CapacityApproximationFailed(
            "Number with length " + std::to_string(p_number_size) +
            " and base " + std::to_string(p_base) +
            " to binary conversion failed");
    }

    return static_cast<uint64_t>(estimated_bits);
}

template<typename InfiniterDerived>
uint64_t InfiniterIO<InfiniterDerived>::estimateCellsByBase(uint64_t p_number_size, uint8_t p_base)
{
    uint64_t estimated_bits = estimateBitsByBase(p_number_size, p_base);

    uint64_t estimated_cells = estimated_bits / BITS_PER_CELL;
    if(estimated_bits % BITS_PER_CELL)
        estimated_cells++;

    return estimated_cells;
}

template<typename InfiniterDerived>
void InfiniterIO<InfiniterDerived>::validateStringNumber(const std::string &p_number, int p_base)
{
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

        if ('0' <= c && c <= '9') {
            if (c - '0' < p_base) is_valid = true;
        }
        else if ('a' <= c && c <= 'z') {
            if (10 + (c - 'a') < p_base) is_valid = true;
        }
        else if ('A' <= c && c <= 'Z') {
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

template<typename InfiniterDerived>
void InfiniterIO<InfiniterDerived>::validateVectorNumber(const std::vector<uint8_t> &p_number, int p_base)
{
    /// empty string
    if (p_number.empty())
    {
        throw InfiniterException::InvalidVectorFormat("Empty vector");
    }

    /// handle leading zeros (01 or 001123 are not allowed)
    if (p_number[0] == 0) {
        throw InfiniterException::InvalidVectorFormat("Leading zeros are not allowed");
    }

    /// main validation
    for (size_t i = 0; i < p_number.size(); ++i) {
        uint8_t value = p_number[i];

        if (value >= p_base) {
            throw InfiniterException::InvalidVectorFormat(
                "Vector contains invalid value '" + std::to_string(value) +
                "' for base " + std::to_string(p_base)
                );
        }
    }
}

template<typename InfiniterDerived>
void InfiniterIO<InfiniterDerived>::assignStringBase2(const std::string &p_binary_number)
{
    /// compute required cells
    uint64_t requiredCells = p_binary_number.size() / BITS_PER_CELL;
    if(p_binary_number.size() % BITS_PER_CELL)
        requiredCells++;

    /// prepare data
    this->reserve(requiredCells);
    this->setSize(requiredCells);
    icell_t *data = this->getData();
    icell_t *data_ptr = data;

    size_t size = p_binary_number.size();
    while(size != 0)
    {
        size_t part = std::min(static_cast<size_t>(BITS_PER_CELL), size);
        icell_t tmp_cell = 0;

        /// for each bit in cell emplace bit from string
        for(size_t i=0; i<part; i++)
        {
            const size_t i_rev = size -part +i;
            const icell_t bit_value =
                (p_binary_number[i_rev] == '0') ? ICELL_C(0) : ICELL_C(1);

            tmp_cell <<= 1;
            tmp_cell |= bit_value;
        }
        size -= part;

        /// emplace cell in data array
        *data_ptr = tmp_cell;
        data_ptr++;
    }
}

template<typename InfiniterDerived>
void InfiniterIO<InfiniterDerived>::assignString(std::string p_number, uint8_t p_base)
{
    std::string binary_number;
    binary_number.reserve( estimateBitsByBase(p_number.size(), p_base) );

    size_t start_index = 0;

    auto shiftBegin = [](size_t &p_start_index, const std::string &p_str){
        while(p_start_index < p_str.size() && p_str[p_start_index] == '0')
        {
            p_start_index++;
        }
    };

    shiftBegin(start_index, p_number);

    while(start_index < p_number.size())
    {
        uint8_t remainder = 0;

        for(size_t i=start_index; i<p_number.size(); i++)
        {
            const char c = p_number[i];
            /// change character to value
            uint8_t value = 0;
            if ('0' <= c && c <= '9') value = c - '0';
            else if ('a' <= c && c <= 'z') value = c - 'a' + 10;
            else if ('A' <= c && c <= 'Z') value = c - 'A' + 10;

            int current_numerator = (remainder * p_base) + value;

            int quotient = current_numerator / 2;
            remainder = current_numerator % 2;

            if(quotient < 10)
                p_number[i] = quotient + '0';
            else
                p_number[i] = quotient - 10 + 'A';
        }

        binary_number.push_back((remainder ? '1' : '0'));

        shiftBegin(start_index, p_number);
    }

    std::reverse(binary_number.begin(), binary_number.end());

    p_number.clear(); /// free some memory

    this->assignStringBase2(binary_number);
}

template<typename InfiniterDerived>
void InfiniterIO<InfiniterDerived>::assignVectorBase2(const std::vector<uint8_t> &p_binary_number)
{
    /// compute required cells
    uint64_t requiredCells = p_binary_number.size() / BITS_PER_CELL;
    if(p_binary_number.size() % BITS_PER_CELL)
        requiredCells++;

    /// prepare data
    this->reserve(requiredCells);
    this->setSize(requiredCells);
    icell_t *data = this->getData();
    icell_t *data_ptr = data;

    size_t size = p_binary_number.size();
    while(size != 0)
    {
        size_t part = std::min(static_cast<size_t>(BITS_PER_CELL), size);
        icell_t tmp_cell = 0;

        /// for each bit in cell emplace bit from string
        for(size_t i=0; i<part; i++)
        {
            const size_t i_rev = size -part +i;
            const icell_t bit_value =
                (p_binary_number[i_rev] == 0) ? ICELL_C(0) : ICELL_C(1);

            tmp_cell <<= 1;
            tmp_cell |= bit_value;
        }
        size -= part;

        /// emplace cell in data array
        *data_ptr = tmp_cell;
        data_ptr++;
    }
}

template<typename InfiniterDerived>
void InfiniterIO<InfiniterDerived>::assignVector(std::vector<uint8_t> p_number, uint8_t p_base)
{
    std::vector<uint8_t> binary_number;
    binary_number.reserve( estimateBitsByBase(p_number.size(), p_base) );

    size_t start_index = 0;

    auto shiftBegin = [](size_t &p_start_index, const std::vector<uint8_t> &p_vec){
        while(p_start_index < p_vec.size() && p_vec[p_start_index] == 0)
        {
            p_start_index++;
        }
    };

    shiftBegin(start_index, p_number);

    while(start_index < p_number.size())
    {
        uint8_t remainder = 0;

        for(size_t i=start_index; i<p_number.size(); i++)
        {
            const int value = p_number[i];

            const int current_numerator = (remainder * p_base) + value;

            const int quotient = current_numerator / 2;
            remainder = current_numerator % 2;

            p_number[i] = quotient;
        }

        binary_number.push_back(remainder ? 1 : 0);
        /// could be better - build icell_t value and if reach msb then push it to vector

        shiftBegin(start_index, p_number);
    }

    std::reverse(binary_number.begin(), binary_number.end());

    p_number.clear(); /// free some memory

    this->assignVectorBase2(binary_number);
}

template<typename InfiniterDerived>
void InfiniterIO<InfiniterDerived>::assign(const std::string &p_number, uint8_t p_base, bool p_negative_value)
{
    /// handles strings where LSB is p_number[p_number.size()-1] and MSB is p_number[0]

    /// limit base
    if (p_base < 2 || p_base > 36)
    {
        throw InfiniterException::InvalidBase(p_base, 2, 36);
    }

    this->validateStringNumber(p_number, p_base);

    if(InfiniterIO::isPowerOfTwo(p_base))
    {
        this->assignStringBase2(p_number);
    }
    else
    {
        assignString(p_number, p_base);
    }

    this->setSign(p_negative_value);
}

template<typename InfiniterDerived>
void InfiniterIO<InfiniterDerived>::assign(const std::vector<uint8_t> &p_number, uint8_t p_base, bool p_negative_value)
{
    /// handles vectors where LSB is p_number[p_number.size()-1] and MSB is p_number[0]

    /// limit base
    if (p_base < 2 || p_base > 36)
    {
        throw InfiniterException::InvalidBase(p_base, 2, 36);
    }

    this->validateVectorNumber(p_number, p_base);

    if(InfiniterIO::isPowerOfTwo(p_base))
    {
        this->assignVectorBase2(p_number);
    }
    else
    {
        assignVector(p_number, p_base);
    }

    this->setSign(p_negative_value);
}

template<typename InfiniterDerived>
std::string InfiniterIO<InfiniterDerived>::toStringBase2() const
{
    isize_t size = this->getSize();
    const icell_t *data = this->getData();

    std::string binary_number;
    binary_number.reserve(size * BITS_PER_CELL);

    bool found_non_zero_cell = false;

    for(isize_t j=0; j<size; j++)
    {
        icell_t cell = data[size -1 -j];

        if(cell == ICELL_C(0) && !found_non_zero_cell)
        {
            continue;
        }

        found_non_zero_cell = true;

        for(int i=0; i<BITS_PER_CELL; i++)
        {
            icell_t bit_mask = ICELL_C(1) << (BITS_PER_CELL -1 -i);
            icell_t bit_masked = cell & bit_mask;
            binary_number.push_back(bit_masked ? '1' : '0');
        }
    }

    /// remove leading zeros
    size_t first_meaningful_value_pos = binary_number.find_first_not_of('0');

    if(first_meaningful_value_pos == std::string::npos)
    {
        return "0";
    }
    else if(first_meaningful_value_pos)
    {
        binary_number.erase(0, first_meaningful_value_pos);
    }


    return binary_number;
}

template<typename InfiniterDerived>
std::vector<uint8_t> InfiniterIO<InfiniterDerived>::toVectorBase2() const
{
    isize_t size = this->getSize();
    const icell_t *data = this->getData();

    std::vector<uint8_t> binary_number;
    binary_number.reserve(size * BITS_PER_CELL);

    bool found_non_zero_cell = false;

    for(isize_t j=0; j<size; j++)
    {
        icell_t cell = data[size -1 -j];

        if(cell == ICELL_C(0) && !found_non_zero_cell)
        {
            continue;
        }

        found_non_zero_cell = true;

        for(int i=0; i<BITS_PER_CELL; i++)
        {
            icell_t bit_mask = ICELL_C(1) << (BITS_PER_CELL -1 -i);
            icell_t bit_masked = cell & bit_mask;
            binary_number.push_back(bit_masked ? '1' : '0');
        }
    }

    /// remove leading zeros
    auto first_meaningful_value_it =
        std::find(binary_number.begin(), binary_number.end(), 1);

    if (first_meaningful_value_it == binary_number.end())
    {
        return {0};
    }
    else if (first_meaningful_value_it != binary_number.begin())
    {
        binary_number.erase(binary_number.begin(), first_meaningful_value_it);
    }

    return binary_number;
}

template<typename InfiniterDerived>
std::string InfiniterIO<InfiniterDerived>::toString(uint8_t p_base) const
{
    /// limit base
    if (p_base < 2 || p_base > 36)
    {
        throw InfiniterException::InvalidBase(p_base, 2, 36);
    }

    std::string binary_number(toStringBase2());

    if (binary_number == "0") {
        return "0";
    }

    std::string number;
    number.reserve(binary_number.size());

    size_t start_index = 0;

    auto shiftBegin = [](size_t &p_start_index, const std::string &p_str){
        while(p_start_index < p_str.size() && p_str[p_start_index] == '0')
        {
            p_start_index++;
        }
    };

    while(start_index < binary_number.size())
    {
        uint32_t remainder = 0;

        for(size_t i = start_index; i < binary_number.size(); i++)
        {
            const uint8_t value = binary_number[i] - '0';
            const uint32_t current_numerator = (remainder * 2) + value;

            const uint8_t quotient = current_numerator / p_base;
            remainder = current_numerator % p_base;

            binary_number[i] = quotient + '0';
        }

        char result_char = (remainder < 10) ? ('0' + remainder) : ('A' + remainder - 10);
        number.push_back(result_char);

        shiftBegin(start_index, binary_number);
    }

    std::reverse(number.begin(), number.end());

    return number;
}

template<typename InfiniterDerived>
std::vector<uint8_t> InfiniterIO<InfiniterDerived>::toVector(uint8_t p_base) const
{
    /// limit base
    if (p_base < 2 || p_base > 36)
    {
        throw InfiniterException::InvalidBase(p_base, 2, 36);
    }

    std::vector<uint8_t> binary_number(toVectorBase2());

    if (binary_number.size() == 1 && binary_number[0] == 0) {
        return {0};
    }

    std::vector<uint8_t> number;
    number.reserve(binary_number.size());

    size_t start_index = 0;

    auto shiftBegin = [](size_t &p_start_index, const std::vector<uint8_t> &p_vec){
        while(p_start_index < p_vec.size() && p_vec[p_start_index] == 0)
        {
            p_start_index++;
        }
    };

    while(start_index < binary_number.size())
    {
        uint32_t remainder = 0;

        for(size_t i = start_index; i < binary_number.size(); i++)
        {
            const uint8_t value = binary_number[i];
            const uint32_t current_numerator = (remainder * 2) + value;

            const uint8_t quotient = current_numerator / p_base;
            remainder = current_numerator % p_base;

            binary_number[i] = quotient;
        }

        number.push_back(static_cast<uint8_t>(remainder));

        shiftBegin(start_index, binary_number);
    }

    std::reverse(number.begin(), number.end());

    return number;
}

// template<typename InfiniterDerived>
// void InfiniterIO<InfiniterDerived>::print(uint64_t p_base) const
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

template<typename InfiniterDerived>
void InfiniterIO<InfiniterDerived>::printBase2() const
{
    const icell_t *data = this->getData();
    const isize_t size = this->getSize();

    char cell_buffer[64+1];

    bool non_zero_cell_found = false;

    for(isize_t i=0; i<size; i++)
    {
        icell_t cell = data[size -i -1];
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

template<typename InfiniterDerived>
void InfiniterIO<InfiniterDerived>::printBase4() const
{

}

template<typename InfiniterDerived>
void InfiniterIO<InfiniterDerived>::printBase8() const
{

}

template<typename InfiniterDerived>
void InfiniterIO<InfiniterDerived>::printBase16() const
{

}

template<typename InfiniterDerived>
void InfiniterIO<InfiniterDerived>::print(uint64_t base) const{
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
