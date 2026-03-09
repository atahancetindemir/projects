// https://en.wikipedia.org/wiki/Dudeney_number

#include <stdio.h>
#include <math.h>

#define N 99
#define UPPER 9999999

unsigned int sum_of_digits(unsigned int n) {
    unsigned int sum = 0;
    while(n != 0) {
        unsigned int remainder = n % 10;
        sum += remainder;
        n /= 10;
    }
    return sum;
}

int main() {
    for (unsigned int i = 0; i < N; i++) {
        printf("x^%u:\t", i);
        for (unsigned int j = 0; j < UPPER; j++) {
            if (j == sum_of_digits(pow(j, i))) {
                printf("%u ", j);
            }
        }
    printf("\n");
    }

    return 0;
}
