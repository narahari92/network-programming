#include<stdio.h>
#include "network_math/network_math.h"

int identify_endian();

int main() {
    return identify_endian();
}

int identify_endian() {
    int endian = little_endian();
    printf("Is little endian: %d\n", endian);

    return 0;
}