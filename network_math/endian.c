#include "network_math.h"

int little_endian() {
    int n = 1;

    return *(char*)&n;
}