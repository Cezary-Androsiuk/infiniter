#include "InfiniterArithmetic.hpp"

#include "InfiniterShared.hpp"

#include "Infiniter.hpp"

#include <utility> // std::move

/// _ia_dbgprintf("--- DEBUG IA %p | Constructed   DEFAULT\n", this);
/// _ia_dbgprintf("--- DEBUG IA %p | Constructed   PARAMETER\n", this);
/// _ia_dbgprintf("--- DEBUG IA %p | Constructed   COPY\n", this);
/// _ia_dbgprintf("--- DEBUG IA %p | Constructed   MOVE\n", this);
/// _ia_dbgprintf("--- DEBUG IA %p | Assigned      COPY\n", this);
/// _ia_dbgprintf("--- DEBUG IA %p | Assigned      MOVE\n", this);
/// _ia_dbgprintf("--- DEBUG IA %p | Delete\n", this);
/// _ia_dbgprintf("--- DEBUG IA %p | Other         (...)\n", this);
#if IA_DEBUG_EXECUTION_PRINT
#include <cstdio>
#define _ia_dbgprintf(...) printf(__VA_ARGS__);
#else // IA_DEBUG_EXECUTION_PRINT
#define _ia_dbgprintf(...)
#endif // IA_DEBUG_EXECUTION_PRINT

InfiniterArithmetic::InfiniterArithmetic() noexcept
    : InfiniterBit()
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed   DEFAULT\n", this);
}

InfiniterArithmetic::InfiniterArithmetic(isize_t p_capacity)
    : InfiniterBit(p_capacity)
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed   PARAMETER isize_t\n", this);
}

InfiniterArithmetic::InfiniterArithmetic(isize_t p_capacity, icell_t p_value, bool p_negative_value)
    : InfiniterBit(p_capacity, p_value, p_negative_value)
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed   PARAMETER isize_t icell_t bool\n", this);
}

InfiniterArithmetic::InfiniterArithmetic(const icell_t *p_array, isize_t p_size, bool p_negative_value)
    : InfiniterBit(p_array, p_size, p_negative_value)
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed   PARAMETER icell_t* isize_t bool\n", this);
}

InfiniterArithmetic::InfiniterArithmetic(const InfiniterArithmetic &p_source)
    : InfiniterBit(p_source)
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed IA COPY\n", this);
}

InfiniterArithmetic::InfiniterArithmetic(InfiniterArithmetic &&p_source) noexcept
    : InfiniterBit(std::move(p_source))
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed IA MOVE\n", this);
}

InfiniterArithmetic::InfiniterArithmetic(const InfiniterBit &p_source)
    : InfiniterBit(p_source)
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed IB COPY\n", this);
}

InfiniterArithmetic::InfiniterArithmetic(InfiniterBit &&p_source) noexcept
    : InfiniterBit(std::move(p_source))
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed IB MOVE\n", this);
}

InfiniterArithmetic::InfiniterArithmetic(const InfiniterCore &p_source)
    : InfiniterBit(p_source)
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed IC COPY\n", this);
}

InfiniterArithmetic::InfiniterArithmetic(InfiniterCore &&p_source) noexcept
    : InfiniterBit(std::move(p_source))
{
    _ia_dbgprintf("--- DEBUG IA %p | Constructed IC MOVE\n", this);
}

InfiniterArithmetic::~InfiniterArithmetic() noexcept
{
    _ia_dbgprintf("--- DEBUG IA %p | Delete\n", this);

    /// everything was done in InfiniterMemory
}

bool InfiniterArithmetic::is0() const noexcept
{
    const icell_t *data = this->getData();
    for(isize_t i=0; i<this->getSize(); i++)
    {
        if(data[i] != ICELL_C(0))
            return false;
    }
    return true;
}

bool InfiniterArithmetic::isPositive1() const noexcept
{
    const icell_t *data = this->getData();
    if(this->getSign())
        return false;

    if(data[0] != ICELL_C(1))
        return false;

    for(isize_t i=1; i<this->getSize(); i++)
    {
        if(data[i] != ICELL_C(0))
            return false;
    }
    return true;
}

bool InfiniterArithmetic::isNegative1() const noexcept
{
    const icell_t *data = this->getData();
    if(!this->getSign())
        return false;

    if(data[0] != ICELL_C(1))
        return false;

    for(isize_t i=1; i<this->getSize(); i++)
    {
        if(data[i] != ICELL_C(0))
            return false;
    }
    return true;
}

Infiniter InfiniterArithmetic::absoluteValue() const
{
    Infiniter i(*this);
    i.setPositiveSign();
    return i;
}

void InfiniterArithmetic::absoluteValueAssign()
{
    this->setPositiveSign();
}

void InfiniterArithmetic::increment()
{
    icell_t *data = this->getData();
    icell_t size = this->getSize();

    icell_t *cell_ptr = data;
    const icell_t *const last_cell = data + (size-1);

    /// find non fill cell
    while(*cell_ptr == ICELL_MAX && cell_ptr != last_cell)
    {
        (*cell_ptr)++; /// 11111111 -> (1)00000000
        cell_ptr++;
    }

    if(cell_ptr == last_cell && *cell_ptr == ICELL_MAX)
    {
        /// reach end and whole number look like this:
        /// 11111111 11111111 ... 11111111 11111111
        /// in the process was changed to this
        /// 00000000 00000000 ... 00000000 00000000
        /// add new cell

        (*cell_ptr)++; /// 11111111 -> (1)00000000

        size = this->setSizeWithExtend(size +1);
        data = this->getData(); /// update data - could change if memory was realocated
        cell_ptr = data + size -1;
    }

    /// cell_ptr will contain cell that could be modifed
    (*cell_ptr)++;
}

void InfiniterArithmetic::decrement()
{
    /// handle decrementing to negative
    if(this->is0())
    {
        this->increment();
        this->setNegativeSign();
        this->trim();
        return;
    }

    icell_t *data = this->getData();
    icell_t size = this->getSize();

    icell_t *cell_ptr = data;
    const icell_t *const last_cell = data + (size-1);


    /// find non fill cell
    while(*cell_ptr == ICELL_C(0) && cell_ptr != last_cell)
    {
        (*cell_ptr)--; /// (1)00000000 -> 11111111
        cell_ptr++;
    }

    /// cell_ptr will contain cell that could be modifed
    (*cell_ptr)--;

    this->trim();
}

void InfiniterArithmetic::assign(const InfiniterArithmetic &p_source)
{
    InfiniterBit::assign(p_source);
}

void InfiniterArithmetic::assign(InfiniterArithmetic &&p_source)
{
    InfiniterBit::assign(std::move(p_source));
}

InfiniterArithmetic &InfiniterArithmetic::operator =(const InfiniterArithmetic &p_source)
{
    InfiniterBit::assign(p_source);
    return *this;
}

InfiniterArithmetic &InfiniterArithmetic::operator ++()
{
    this->increment();
    return *this;
}

InfiniterArithmetic InfiniterArithmetic::operator ++(int)
{

}

InfiniterArithmetic &InfiniterArithmetic::operator --()
{

}

InfiniterArithmetic InfiniterArithmetic::operator --(int)
{

}

InfiniterArithmetic &InfiniterArithmetic::operator =(InfiniterArithmetic &&p_source)
{
    InfiniterBit::assign(std::move(p_source));
    return *this;
}


