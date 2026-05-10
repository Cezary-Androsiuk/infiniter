#include "source/InfiniterMemory.h"
#include "source/InfiniterCore.h"
#include "source/InfiniterIO.h"
#include <cstdio>

/*
 * Infiniter
 * InfiniterMath
 * InfiniterBit (InfiniterLow)
 * InfiniterIO
 * InfiniterCore
 */

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

    cell_t m_memory[8];
    for(int i=0; i<8; i++) m_memory[i] = i+1;
    print_array(m_memory, 8);


    cell_t *tmp_memory = new cell_t[10];
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
    InfiniterMemory im0;
    im0.dbg_print();

    InfiniterMemory im(30);
    im.dbg_print();

    printf("\n");

    InfiniterMemory im2(im);
    im2.dbg_print();

    printf("\n");

    InfiniterMemory im3(std::move(im));
    im3.dbg_print();
    im.dbg_print();
    im2.dbg_print();

    printf("\n");

    im = im0;
    im.dbg_print();
    im = im2;
    im.dbg_print();
    im = std::move(im0);
    im.dbg_print();
    im0.dbg_print();


    printf("\n");
    printf("\n");


    // InfiniterMemory im10;
    // im10.reserve(SBO_CAPACITY+5);
    // im10.m_memory[im10.m_capacity-1-2] = -1;
    // im10.dbg_print();
    // im10.shrink();
    // im10.dbg_print();

    {
        InfiniterMemory _im(SBO_CAPACITY +5);
        _im.m_memory[SBO_CAPACITY +1] = -1;
        _im.dbg_print();
        _im.shrink();
        _im.dbg_print();
        printf("\n");
    }

    {
        InfiniterMemory _im(SBO_CAPACITY +5);
        _im.m_memory[SBO_CAPACITY] = -1;
        _im.dbg_print();
        _im.shrink();
        _im.dbg_print();
        printf("\n");
    }

    {
        InfiniterMemory _im(SBO_CAPACITY +5);
        _im.m_memory[SBO_CAPACITY -1] = -1;
        _im.dbg_print();
        _im.shrink();
        _im.dbg_print();
        printf("\n");
    }

    {
        InfiniterMemory _im(SBO_CAPACITY +5);
        _im.m_memory[SBO_CAPACITY -2] = -1;
        _im.dbg_print();
        _im.shrink();
        _im.dbg_print();
        printf("\n");
    }



    /// r-value (move) / l-value

    // test_reasigning_data_to_new_array();

    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");

}

int main(int argc, char *argv[])
{
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

    infiniterMemoryTests();


}
