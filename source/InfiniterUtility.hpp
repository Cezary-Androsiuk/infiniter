#ifndef INFINITERUTILITY_HPP
#define INFINITERUTILITY_HPP

#include "InfiniterShared.hpp"
#include "InfiniterIO.hpp"

#include <string>
#include <vector>

class InfiniterUtility : public InfiniterIO
{
public:
    explicit InfiniterUtility() noexcept;

    explicit InfiniterUtility(isize_t p_capacity); /// throws bad_alloc
    explicit InfiniterUtility(isize_t p_capacity, icell_t p_value, bool p_negative_value=false); /// throws bad_alloc
    explicit InfiniterUtility(const icell_t *p_array, isize_t p_size, bool p_negative_value=false); /// throws bad_alloc

    explicit InfiniterUtility(const std::string &p_number, int p_base=10, bool p_negative_value=false); /// throws bad_alloc /// InfiniterException::InvalidStringFormat
    explicit InfiniterUtility(const std::vector<uint8_t> &p_number, int p_base=10, bool p_negative_value=false); /// throws bad_alloc

    InfiniterUtility(const InfiniterUtility &p_source); /// throws bad_alloc
    InfiniterUtility(InfiniterUtility &&p_source) noexcept;

    InfiniterUtility(const InfiniterIO &p_source); /// throws bad_alloc
    InfiniterUtility(InfiniterIO &&p_source) noexcept;

    InfiniterUtility(const InfiniterArithmetic &p_source); /// throws bad_alloc
    InfiniterUtility(InfiniterArithmetic &&p_source) noexcept;

    InfiniterUtility(const InfiniterBit &p_source); /// throws bad_alloc
    InfiniterUtility(InfiniterBit &&p_source) noexcept;

    InfiniterUtility(const InfiniterCore &p_source); /// throws bad_alloc
    InfiniterUtility(InfiniterCore &&p_source) noexcept;

    ~InfiniterUtility() noexcept;

    /// InfiniterIO methods

    using InfiniterIO::assign;
    void assign(const InfiniterUtility &p_source);
    void assign(InfiniterUtility &&p_source);

public:

    /// Operators
    using InfiniterIO::operator = ;
    InfiniterUtility &operator = (const InfiniterUtility &p_source);
    InfiniterUtility &operator = (InfiniterUtility &&p_source);
};

#endif // INFINITERUTILITY_HPP
