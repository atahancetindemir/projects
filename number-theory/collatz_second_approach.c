#include <stdio.h>
#include <stdint.h>

#define LOWER 68719476736 // Calculated numbers from 1 to N that proves collatz sequence.
#define UPPER LOWER * 2

// Computes the Collatz sequence for numbers within a given range.
void collatz(uint64_t lower, uint64_t upper) {
    for(uint64_t i = lower; i < upper; i++) {
        uint64_t x = i;
        while(x != 4) {
            if (x < i) break;
            x >>= __builtin_ctzll(x);
            x = 3 * x + 1;
        }
    }
}

int main() {
    collatz(LOWER, UPPER);
    return 0;
}
