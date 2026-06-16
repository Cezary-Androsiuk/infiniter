#pragma once

#include <cstdint>
#include <climits> // CHAR_BIT

// masks
#define M000 (0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000)
#define M001 (0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000001)
#define M010 (0b01111111'11111111'11111111'11111111'11111111'11111111'11111111'11111110)
#define M011 (0b01111111'11111111'11111111'11111111'11111111'11111111'11111111'11111111)
#define M100 (0b10000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000)
#define M101 (0b10000000'00000000'00000000'00000000'00000000'00000000'00000000'00000001)
#define M110 (0b11111111'11111111'11111111'11111111'11111111'11111111'11111111'11111110)
#define M111 (0b11111111'11111111'11111111'11111111'11111111'11111111'11111111'11111111)

typedef uint64_t    icell_t;    /// cell capacity
typedef uint64_t    isize_t;    /// size capacity
typedef uint8_t     ibit_t;     /// bit capacity

#define IBIT_1 UINT8_C(1)
#define IBIT_0 UINT8_C(0)
#define IBIT_TRUE IBIT_1
#define IBIT_FALSE IBIT_0

#define ICELL_C(x) UINT64_C(x)
#define ISIZE_C(x) UINT64_C(x)
#define IBIT_C(x) (x ? IBIT_TRUE : IBIT_FALSE)

#define ICELL_MAX UINT64_MAX
#define ISIZE_MAX UINT64_MAX

inline constexpr std::size_t icell_bits = sizeof(icell_t) * CHAR_BIT;
inline constexpr std::size_t isize_bits = sizeof(isize_t) * CHAR_BIT;

/// WARNING: providing 0 value will couse undefined behaviour
/// clzll stands for "Count Leading Zeros Long Long"
#define ICELL_MSB_POS(CELL) ((icell_bits-1) - __builtin_clzll(CELL))
/// clzll stands for "Count Trailing Zeros Long Long"
#define ICELL_LSB_POS(CELL) (__builtin_ctzll(CELL))
/// clzll stands for "Count Leading Zeros Long Long"
#define ICELL_SAFE_MSB_POS(CELL) (CELL ? ((icell_bits-1) - __builtin_clzll(CELL)) : ICELL_C(0))
/// clzll stands for "Count Trailing Zeros Long Long"
#define ICELL_SAFE_LSB_POS(CELL) (CELL ? (__builtin_ctzll(CELL)) : ICELL_C(0))


/// Can't be less than 1
#define SBO_CAPACITY 3



#define ENABLE_MEMORY_ALLOCATION_PADDING true
#define USE_DYNAMIC_PADDING_SIZE true

#if ENABLE_MEMORY_ALLOCATION_PADDING

    #if USE_DYNAMIC_PADDING_SIZE
        #define _REALLOC_CONSTANT 1.0
        #define REALLOC_PADDING_SIZE( CURRENT_SIZE ) ((CURRENT_SIZE) + ((CURRENT_SIZE) * _REALLOC_CONSTANT))
    #else // USE_DYNAMIC_PADDING_SIZE
        #define _REALLOC_CONSTANT 1024
        #define REALLOC_PADDING_SIZE( CURRENT_SIZE ) ((CURRENT_SIZE) + _REALLOC_CONSTANT)
    #endif // USE_DYNAMIC_PADDING_SIZE

#else // ENABLE_MEMORY_ALLOCATION_PADDING

    #define _REALLOC_CONSTANT 0
    #define REALLOC_PADDING_SIZE( CURRENT_SIZE ) (CURRENT_SIZE)

#endif // ENABLE_MEMORY_ALLOCATION_PADDING



union BitsUS{ /// bits union struct
    uint8_t _byte = 0; /// = 0 ensures clear bit space while initialization
    struct {
        ibit_t sbo_active  : 1; // IM
        ibit_t sign        : 1; // IC
        // ibit_t
        // ibit_t
        // ibit_t
        // ibit_t
        // ibit_t
        // ibit_t
    };
};


/// Little Endian aproach


/// allows to enable highlited syntax  in tpp files
#define INFINITER_TPP_INCLUDED false



// ####################### INFINITER MEMORY #######################

/// Infiniter variable should be set to 0 after allocation? or leave it without overwriting old data
/// after allocation memory will be set to 0 if enabled, if disabled memory can have trash in in
#define IM_CLEAR_ALLOCATED_MEMORY true

/// if not ensured, object after std::move will be corrupted and need to be deleted, but it is few operation faster
#define IM_ENSURE_NEW_OBJECT_AFTER_MOVE true

/// prints what is happening - what constructors are executed
#define IM_DEBUG_EXECUTION_PRINT true

/// declares method that can be used by friendly functions to display raw memory in hex
#define IM_ENABLE_DB_PRINT_METHOD true

/// specify loop stop value and max capacity of InfiniterMemory array, it is important because 'for' loop require stop value (in that case stop value is 0b111...111 or UINT64_MAX or (~UINT64_C(0)))
#define IM_CAPACITY_LOOP_STOP ISIZE_MAX
#define IM_MAX_CAPACITY (IM_CAPACITY_LOOP_STOP - 1)

// ^^^^^^^^^^^^^^^^^^^^^^^ INFINITER MEMORY ^^^^^^^^^^^^^^^^^^^^^^^



// ####################### INFINITER CORE #######################

/// prints what is happening - what constructors are executed
#define IC_DEBUG_EXECUTION_PRINT true

/// declares method that can be used by friendly functions to display raw memory in hex
#define IC_ENABLE_DB_PRINT_METHOD true

// ^^^^^^^^^^^^^^^^^^^^^^^ INFINITER CORE ^^^^^^^^^^^^^^^^^^^^^^^



// ####################### INFINITER BIT #######################

/// prints what is happening - what constructors are executed
#define IB_DEBUG_EXECUTION_PRINT true

// ^^^^^^^^^^^^^^^^^^^^^^^ INFINITER BIT ^^^^^^^^^^^^^^^^^^^^^^^



// ####################### INFINITER ARITHMETIC #######################

/// prints what is happening - what constructors are executed
#define IA_DEBUG_EXECUTION_PRINT true

// ^^^^^^^^^^^^^^^^^^^^^^^ INFINITER ARITHMETIC ^^^^^^^^^^^^^^^^^^^^^^^



// ####################### INFINITER IO #######################

/// prints what is happening - what constructors are executed
#define IO_DEBUG_EXECUTION_PRINT true

#define IO_MAX_CONTAINER_INPUT_CAPACITY static_cast<size_t>(ICELL_MAX >> 16)

// ^^^^^^^^^^^^^^^^^^^^^^^ INFINITER IO ^^^^^^^^^^^^^^^^^^^^^^^



// ####################### INFINITER UTILITY #######################

/// prints what is happening - what constructors are executed
#define IU_DEBUG_EXECUTION_PRINT true

// ^^^^^^^^^^^^^^^^^^^^^^^ INFINITER UTILITY ^^^^^^^^^^^^^^^^^^^^^^^

#define LIKELY(x)       __builtin_expect(!!(x), 1)
#define UNLIKELY(x)     __builtin_expect(!!(x), 0)
