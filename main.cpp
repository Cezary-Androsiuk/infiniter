#include "source/InfiniterCore.h"
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
    InfiniterCore icp;
    icp.dbgPrint(); printf("\n");

    icp.reserve(0);
    icp.dbgPrint(); printf("\n");

    icp.reserve(1);
    icp.dbgPrint(); printf("\n");

    icp.reserve(2);
    icp.dbgPrint(); printf("\n");

    icp.reserve(3);
    icp.dbgPrint(); printf("\n");

    icp.reserve(4);
    icp.dbgPrint(); printf("\n");

    icp.reserve(3);
    icp.dbgPrint(); printf("\n");



}
