#include "source/InfiniterMemory.hpp"
#include "source/InfiniterCore.hpp"
// #include "source/InfiniterIO.hpp"
// #include "source/InfiniterUtility.hpp"
#include "source/Infiniter.hpp"

#include <cstdio>
#include <algorithm> // std::copy_n, std::fill_n, std::move
#include <cassert>



/*
 * InfiniterException   IE
 * InfiniterShared      IS
 * InfiniterTests       IT
 * InfiniterDerived     ID - template
 *
 *
 * Infiniter            I
 * InfiniterUtility     IU
 * InfiniterIO          IO
 * InfiniterArithmetic  IA
 * InfiniterBit         IB
 * InfiniterCore        IC
 * InfiniterMemory      IM
 */
/*
 * InfiniterException   IE
 * InfiniterShared      IS
 * InfiniterTests       IT
 * InfiniterDerived     ID - template
 *
 *
 * Infiniter : public InfiniterUtility          I
 * InfiniterUtility : public InfiniterIO        IU
 * InfiniterIO : public InfiniterArithmetic     IO
 * InfiniterArithmetic : public InfiniterBit    IA
 * InfiniterBit : public InfiniterCore          IB
 * InfiniterCore : private InfiniterMemory      IC
 * InfiniterMemory                              IM
 */



///
///         COUNT SINGLE CPU STEPS
///

#include <x86intrin.h> // GCC/Clang only // for MSVC (Windows) use <intrin.h>

inline uint64_t read_tsc_start() {
    _mm_lfence(); // Czeka, aż wszystkie wcześniejsze instrukcje zostaną zakończone
    return __rdtsc();
}

// Funkcja końcowa z barierą wyjścia
inline uint64_t read_tsc_end() {
    unsigned int aux;
    uint64_t tsc = __rdtscp(&aux); // rdtscp jest instrukcją serializującą (czeka na kod przed nią)
    _mm_lfence(); // Zapobiega przeskoczeniu późniejszych instrukcji przed rdtscp
    return tsc;
}




void print_array(uint64_t *array, uint64_t size)
{

    printf("size: %llu\n", size);
    for(int i=0; i<size; i++)
    {
        printf("%016llx ", array[size-1-i]);
    }
    printf("\n");
}

void test_reasigning_data_to_new_array()
{

    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");

    /// testing copying and filling what left

    icell_t m_memory[8];
    for(int i=0; i<8; i++) m_memory[i] = i+1;
    print_array(m_memory, 8);


    icell_t *tmp_memory = new icell_t[10];
    for(int i=0; i<10; i++) tmp_memory[i] = UINT64_C(-1);
    print_array(tmp_memory, 10);

    std::copy_n(m_memory, 8, tmp_memory);
    print_array(tmp_memory, 10);

    uint64_t addition = 10-8; // p_new_capacity-m_capacity
    std::fill_n(tmp_memory+8, addition, 0xaaaaaaaaaaaaaaaa);
    print_array(tmp_memory, 10);
}


void infiniterMemoryTests()
{
    printf("==================================================\n");
    InfiniterMemory im0;
    im_dbg_print(im0);

    InfiniterMemory im(30);
    im_dbg_print(im);

    printf("\n");

    InfiniterMemory im2(im);
    im_dbg_print(im2);

    printf("\n");

    InfiniterMemory im3(std::move(im));
    im_dbg_print(im3);
    im_dbg_print(im);
    im_dbg_print(im2);

    printf("\n");

    im = im0;
    im_dbg_print(im);
    im = im2;
    im_dbg_print(im);
    im = std::move(im0);
    im_dbg_print(im);
    im_dbg_print(im0);


    printf("\n");
    printf("\n");


    // InfiniterMemory im10;
    // im10.reserve(SBO_CAPACITY+5);
    // im10.m_memory[im10.m_capacity-1-2] = -1;
    // im10.dbg_print();
    // im10.shrink();
    // im10.dbg_print();

    // {
    //     InfiniterMemory _im(SBO_CAPACITY +5);
    //     _im.m_memory[SBO_CAPACITY +1] = -1;
    //     im_dbg_print(_im);
    //     _im.shrink();
    //     im_dbg_print(_im);
    //     printf("\n");
    // }

    // {
    //     InfiniterMemory _im(SBO_CAPACITY +5);
    //     _im.m_memory[SBO_CAPACITY] = -1;
    //     im_dbg_print(_im);
    //     _im.shrink();
    //     im_dbg_print(_im);
    //     printf("\n");
    // }

    // {
    //     InfiniterMemory _im(SBO_CAPACITY +5);
    //     _im.m_memory[SBO_CAPACITY -1] = -1;
    //     im_dbg_print(_im);
    //     _im.shrink();
    //     im_dbg_print(_im);
    //     printf("\n");
    // }

    // {
    //     InfiniterMemory _im(SBO_CAPACITY +5);
    //     _im.m_memory[SBO_CAPACITY -2] = -1;
    //     im_dbg_print(_im);
    //     _im.shrink();
    //     im_dbg_print(_im);
    //     printf("\n");
    // }



    /// r-value (move) / l-value

    // test_reasigning_data_to_new_array();

    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");

}

void infiniterCoreTests()
{
    printf("==================================================\n");
    Infiniter ic0;
    ic_dbg_print_memory(ic0);
    ic0.assign(0xABCDEF, false);
    ic_dbg_print_memory(ic0);

    Infiniter ic1(20, 0x1234, true);
    ic_dbg_print_memory(ic1);
    ic1.getData()[1] = 123;
    ic_dbg_print_memory(ic1);

    uint64_t array[] = {
        0,1,2,3,4,5,6,7,8,9,
        10,11,12,13,14,15,16,17,18,19,
        20,21,22,23,24,25,26,27,28,29,
        30,31,32,33,34,35,36,37,38,39,
        40,41,42,43,44,45,46,47,48,49,
        50,51,52,53,54,55,56,57,58,59,
        60,61,62,63,64,65,66,67,68,69,
        70,71,72,73,74,75,76,77,78,79,
        80,81,82,83,84,85,86,87,88,89,
        90,91,92,93,94,95,96,97,98,99};

    ic1.assign(array, sizeof(array)/sizeof(uint64_t), true);
    ic_dbg_print_memory(ic1);
    ic1.optimize();
    ic_dbg_print_memory(ic1);
    ic1.reset();
    ic_dbg_print_memory(ic1);
    ic1.reserve(100);
    ic_dbg_print_memory(ic1);
    ic1.assign(array, sizeof(array)/sizeof(uint64_t), true);
    ic_dbg_print_memory(ic1);

    ic1 = ic0;
    ic1.operator=(ic0);
    ic_dbg_print_memory(ic1);
    ic1.getData()[1] = 0x12345;
    ic_dbg_print_memory(ic1);
    ic0 = std::move(ic1);
    ic_dbg_print_memory(ic0);
    ic_dbg_print_memory(ic1);

    printf("\n\n\n");
    ic1.reset();
    ic_dbg_print_memory(ic1);

    ic1.setSize(2);
    ic1[0] = 1ull << 60;
    ic1[1] = 1ull << 60;
    ic_dbg_print_memory(ic1);

    isize_t a0_0[] = { 1, 2, 3 };
    isize_t a1_0[] = { 1, 2, 3 };

    isize_t a0_1[] = { 2, 2, 3 };
    isize_t a1_1[] = { 1, 2, 3 };

    isize_t a0_2[] = { 1, 2, 3 };
    isize_t a1_2[] = { 2, 2, 3 };

    isize_t a0_3[] = { 1, 3, 3 };
    isize_t a1_3[] = { 1, 2, 3 };

    isize_t a0_4[] = { 1, 2, 3 };
    isize_t a1_4[] = { 1, 3, 3 };

    isize_t a0_5[] = { 1, 2, 3 };
    isize_t a1_5[] = { 2, 3 };

    isize_t a0_6[] = { 2, 3 };
    isize_t a1_6[] = { 1, 2, 3 };

    ic0.assign(a0_0, 3);
    ic1.assign(a1_0, 3);
    assert( ic0.equal(ic1) );
    assert(! ic0.differs(ic1) );
    assert(! ic0.greater(ic1) );
    assert(! ic0.smaller(ic1) );
    assert( ic0.greaterEqual(ic1) );
    assert( ic0.smallerEqual(ic1) );

    ic0.assign(a0_1, 3);
    ic1.assign(a1_1, 3);
    assert(! ic0.equal(ic1) );
    assert( ic0.differs(ic1) );
    assert( ic0.greater(ic1) );
    assert(! ic0.smaller(ic1) );
    assert( ic0.greaterEqual(ic1) );
    assert(! ic0.smallerEqual(ic1) );

    ic0.assign(a0_2, 3);
    ic1.assign(a1_2, 3);
    assert(! ic0.equal(ic1) );
    assert( ic0.differs(ic1) );
    assert(! ic0.greater(ic1) );
    assert( ic0.smaller(ic1) );
    assert(! ic0.greaterEqual(ic1) );
    assert( ic0.smallerEqual(ic1) );

    ic0.assign(a0_3, 3);
    ic1.assign(a1_3, 3);
    assert(! ic0.equal(ic1) );
    assert( ic0.differs(ic1) );
    assert( ic0.greater(ic1) );
    assert(! ic0.smaller(ic1) );
    assert( ic0.greaterEqual(ic1) );
    assert(! ic0.smallerEqual(ic1) );

    ic0.assign(a0_4, 3);
    ic1.assign(a1_4, 3);
    assert(! ic0.equal(ic1) );
    assert( ic0.differs(ic1) );
    assert(! ic0.greater(ic1) );
    assert( ic0.smaller(ic1) );
    assert(! ic0.greaterEqual(ic1) );
    assert( ic0.smallerEqual(ic1) );

    ic0.assign(a0_5, 3);
    ic1.assign(a1_5, 3);
    assert(! ic0.equal(ic1) );
    assert( ic0.differs(ic1) );
    assert( ic0.greater(ic1) );
    assert(! ic0.smaller(ic1) );
    assert( ic0.greaterEqual(ic1) );
    assert(! ic0.smallerEqual(ic1) );

    ic0.assign(a0_6, 3);
    ic1.assign(a1_6, 3);
    assert(! ic0.equal(ic1) );
    assert( ic0.differs(ic1) );
    assert(! ic0.greater(ic1) );
    assert( ic0.smaller(ic1) );
    assert(! ic0.greaterEqual(ic1) );
    assert( ic0.smallerEqual(ic1) );


}


// void infiniterIOTests()
// {
//     printf("==================================================\n");
//     InfiniterIO io0(0x1, false);
//     io0.dbg_print_memory();

//     // for(int j=0; j<40; j++)
//     // {
//     //     io0.assign(1, false);

//     //     uint64_t warm1_start = read_tsc_start();
//     //     uint64_t warm1_end = read_tsc_end();
//     //     uint64_t warm2_start = read_tsc_start();
//     //     uint64_t warm2_end = read_tsc_end();

//     //     uint64_t start = read_tsc_start();

//     //     for(int i=0; i<64*10*j; i++)
//     //     {
//     //         io0.pushLSB(1);
//     //     }

//     //     uint64_t end = read_tsc_end();

//     //     // printf("Warm 1 time: %llu\n", warm1_end - warm1_start);
//     //     // printf("Warm 2 time: %llu\n", warm2_end - warm2_start);
//     //     printf("Iterations: %d\n", j*10);
//     //     printf("Clock cycles: %llu\n", end - start);
//     // }

//     // {
//     //     uint64_t warm1_start = read_tsc_start();
//     //     uint64_t warm1_end = read_tsc_end();
//     //     uint64_t warm2_start = read_tsc_start();
//     //     uint64_t warm2_end = read_tsc_end();

//     //     uint64_t start = read_tsc_start();

//     //     for(int i=0; i<64*100; i++)
//     //     {
//     //         io0.pushLSB(1);
//     //     }

//     //     uint64_t end = read_tsc_end();

//     //     printf("Warm 1 time: %llu\n", warm1_end - warm1_start);
//     //     printf("Warm 2 time: %llu\n", warm2_end - warm2_start);
//     //     printf("Clock cycles: %llu\n", end - start);
//     // }
//     // {
//     //     uint64_t warm1_start = read_tsc_start();
//     //     uint64_t warm1_end = read_tsc_end();
//     //     uint64_t warm2_start = read_tsc_start();
//     //     uint64_t warm2_end = read_tsc_end();

//     //     uint64_t start = read_tsc_start();

//     //     for(int i=0; i<64*1'000; i++)
//     //     {
//     //         io0.pushLSB(1);
//     //     }

//     //     uint64_t end = read_tsc_end();

//     //     printf("Warm 1 time: %llu\n", warm1_end - warm1_start);
//     //     printf("Warm 2 time: %llu\n", warm2_end - warm2_start);
//     //     printf("Clock cycles: %llu\n", end - start);
//     // }
//     // {
//     //     uint64_t warm1_start = read_tsc_start();
//     //     uint64_t warm1_end = read_tsc_end();
//     //     uint64_t warm2_start = read_tsc_start();
//     //     uint64_t warm2_end = read_tsc_end();

//     //     uint64_t start = read_tsc_start();

//     //     for(int i=0; i<64*100'000; i++)
//     //     {
//     //         io0.pushLSB(1);
//     //     }

//     //     uint64_t end = read_tsc_end();

//     //     printf("Warm 1 time: %llu\n", warm1_end - warm1_start);
//     //     printf("Warm 2 time: %llu\n", warm2_end - warm2_start);
//     //     printf("Clock cycles: %llu\n", end - start);
//     // }
//     // io0.dbg_print_memory();



//     io0.assign(0x1);
//     for(int i=0; i<64*3; i++)
//     {
//         io0.pushLSB(0);
//         io0.print(10);
//         printf("\n%s\n", io0.toString(10).c_str());
//         printf("%s\n", io0.toString(36).c_str());
//         io0.printBase2();
//         printf("\n%s\n", io0.toString(2).c_str());
//         printf("\n");
//     }

//     printf("\n\n");

//     io0.assign(2);
//     for(int i=0; i<64*2; i++)
//     {
//         io0.pushMSB(1);
//         printf("%s\n", io0.toString(2).c_str());
//     }


//     printf("\n\n");

//     printf("%s\n", io0.toString(36).c_str());
//     io0.serialize("io0-large.inf");
//     io0.deserialize("io0-large.inf");
//     printf("%s\n", io0.toString(36).c_str());

//     printf("\n\n");

//     ic_dbg_print_memory(io0);
//     ic_dbg_print_data(io0);

//     io0.clear();

//     ic_dbg_print_memory(io0);
//     ic_dbg_print_data(io0);

//     io0.assign(0);

//     ic_dbg_print_memory(io0);
//     ic_dbg_print_data(io0);

//     io0.reset();

//     ic_dbg_print_memory(io0);
//     ic_dbg_print_data(io0);

//     printf("\n\n");

//     // const char *binary_str = "111110000111100001111000011110000111100001111000011110000111100000001";
//     const char *binary_str = "11100111";

//     io0.assignStringBase2(binary_str);
//     ic_dbg_print_data(io0);


// }


int main(int argc, char *argv[])
{

    Infiniter infiniter1("12");
    printf("\n");
    Infiniter infiniter2("48");
    printf("\n");

    infiniter1.print(10);
    printf("\n");
    infiniter2.print(10);
    printf("\n");

    infiniter1.add(infiniter2);
    printf("\n");

    printf("\n");
    infiniter1.print(10);
    printf("\n");
    printf("\n");


    Infiniter infiniter3("12345");
    printf("\n");
    infiniter3.print(10);
    printf("\n");
    infiniter3.print(2);
    printf("\n");
    printf("\n");


    infiniter1.print(10);
    printf("\n");

    Infiniter infiniter4 = Infiniter::multiply(infiniter1, infiniter3);
    printf("\n");
    printf("\n");
    infiniter4.print(10);
    printf("\n");
    infiniter4.print(2);
    printf("\n");

    Infiniter infiniter5 = Infiniter::multiply(infiniter3, infiniter1);
    printf("\n");
    printf("\n");
    infiniter5.print(10);
    printf("\n");
    infiniter5.print(2);
    printf("\n");

    const icell_t i5_arr[] = {1,2,3,4,5,6,7,0,0};
    infiniter5.assign(i5_arr, sizeof(i5_arr)/sizeof(icell_t), false);
    // infiniter5.print(2);
    infiniter5.dbg_print_data();
    printf("\n");
    infiniter5.setSizeWithExtend(sizeof(i5_arr)/sizeof(icell_t));
    // infiniter5.print(2);
    infiniter5.dbg_print_data();
    printf("\n");
    infiniter5.shiftCellsLeft(1, ~ICELL_C(0));
    // infiniter5.print(2);
    infiniter5.dbg_print_data();
    printf("\n");
    infiniter5.shiftCellsLeft(3, ICELL_C(0));
    // infiniter5.print(2);
    infiniter5.dbg_print_data();
    printf("\n");

    // InfiniterIO icp;
    // icp.dbgPrint(); printf("\n");

    // icp.reserve(0);
    // icp.dbgPrint(); printf("\n");

    // icp.reserve(1);
    // icp.dbgPrint(); printf("\n");

    // icp.reserve(2);
    // icp.dbgPrint(); printf("\n");

    // icp.reserve(3);
    // icp.dbgPrint(); printf("\n");

    // icp.reserve(4);
    // icp.dbgPrint(); printf("\n");

    // icp.reserve(3);
    // icp.dbgPrint(); printf("\n");

    // icp.getData()[1]=1;
    // printf("%llu\n", ~UINT64_C(0));
    // icp.print(10);

    // cell * c = icp.getData();
    // printf("\n\n%p\n", c);
    // printf("%llu\n", *c);
    // printf("\n\n%p\n", c+1);
    // printf("%llu\n", *(c+1));
    // printf("\n\n%p\n", &(c[1]));
    // printf("%llu\n", c[1]);

    // icp.getData()[2]=1024+512+256+1;

    // icp.dbgPrint();
    // printf("\n");
    // icp.serialize("serialized.txt");

    // InfiniterIO io;
    // io.dbgPrint();
    // printf("\n");
    // io.deserialize("serialized.txt");
    // io.dbgPrint();
    // printf("\n");

    // infiniterMemoryTests();

    // infiniterCoreTests();

    // infiniterIOTests();

    // InfiniterIO io0;
    // InfiniterUtility iu0(io0);
    // InfiniterUtility *iu1 = ++iu0;
    // if()

}


// 111
// 111
// 110 001

// 1111
// 1111
// 1110 0001

// 11111
// 11111
// 11110 00001

// 111111
// 111111
// 111110 000001


// 18,446,744,073,709,551,615
// 18446744073709551615
// 18446744073709551615 * 64 =
// 1180591620717411303360

// 18446744073709551616 * 64 =
// 1180591620717411303360 + 64

// 2^70 =
// 1180591620717411303360

// 2^64 =
// 18446744073709551616
// 2^128 =
// 340282366920938463463374607431768211456
