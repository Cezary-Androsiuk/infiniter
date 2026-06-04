#include "InfiniterIO.hpp"

#include "InfiniterShared.hpp"

#include <utility> // std::move
#include <string>
#include <vector>


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
{
    _io_dbgprintf("--- DEBUG IO %p | Constructed   PARAMETER std::string int bool\n", this);
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
{
    _io_dbgprintf("--- DEBUG IO %p | Constructed   COPY\n", this);

}

InfiniterIO::InfiniterIO(InfiniterIO &&p_source) noexcept
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

void InfiniterIO::assign(uint64_t p_value, bool p_negative_value) noexcept
{
    InfiniterCore::assign(p_value, p_negative_value);
}

void InfiniterIO::assign(const cell_t *p_array, uint64_t p_size, bool p_negative_value)
{
    InfiniterCore::assign(p_array, p_size, p_negative_value);
}

void InfiniterIO::assign(const std::string &p_number, int p_base, bool p_negative_value)
{
    if(InfiniterIO::isPowerOfTwo(p_base))
    {

    }
    else
    {

    }
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



