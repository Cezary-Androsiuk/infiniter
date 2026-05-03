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

    InfiniterMemory im;

    im.print();
}
