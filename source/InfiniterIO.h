#ifndef INFINITERIO_H
#define INFINITERIO_H

#include "InfiniterCore.h"

#include <string>

class InfiniterIO : public InfiniterCore
{
public:
    InfiniterIO();

    void serialize(std::string file_path) const;
    void deserialize(std::string file_path);

    void print(uint64_t base) const;
};

#endif // INFINITERIO_H
