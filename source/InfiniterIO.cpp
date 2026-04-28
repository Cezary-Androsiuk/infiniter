#include "InfiniterIO.h"

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

InfiniterIO::InfiniterIO()
    : InfiniterCore()
{

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

    fwrite(this->getData(), sizeof(cell), this->getSize(), file);

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

    // check file size
    fseek(file, 0, SEEK_END);
    uint64_t fsize = ftell(file);
    rewind(file);

    if(fsize % sizeof(cell) != 0 || fsize == 0)
    {
        fprintf(stderr, "Error: Invalid file format in \"%s\"!\n", file_path.c_str());
        fclose(file);
        return;
    }

    this->clear();
    this->reserve(fsize / sizeof(cell) );

    uint64_t bytes_read = fread(this->getData(), sizeof(cell), fsize, file);

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

