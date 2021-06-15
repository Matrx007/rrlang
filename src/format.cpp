#pragma once

#include <cstdio>
#include <limits.h>

int numPlaces (int n) {
    if (n < 0) return numPlaces ((n == INT_MIN) ? INT_MAX: -n);
    if (n < 10) return 1;
    return 1 + numPlaces (n / 10);
}

void printIndent(int places) {
    for(int i = 0; i < places; i++) {
        fputc(' ', stdout);
    }
}