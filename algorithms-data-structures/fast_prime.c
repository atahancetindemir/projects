#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

bool is_prime(uint64_t n, uint64_t* total_iter, uint32_t* factor) {
    if(n % 2 == 0) {
        (*factor) = 2;
        return false;
    }
    for(uint64_t i = 3; i < sqrt(n); i += 2) {
        (*total_iter)++;
        if(n % i == 0) {
            (*factor) = i;
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    uint64_t n;
    uint64_t total_iter = 0;
    uint32_t factor = 0;
    bool check;
    
    if(argc == 2) {
        sscanf(argv[1], "%lu", &n);
    }
    else {
        printf("usage: %s {n}\n{n}: 64 bit unsigned integer\n", argv[0]);
        return 1;
    }

    clock_t start = clock();
    check = is_prime(n, &total_iter, &factor);
    clock_t end = clock();
    check ? printf("prime\ntotal iteration: %lu\nfactor: %u\n", total_iter, factor)
    : printf("not prime\ntotal iteration: %lu\nfactor: %u\n", total_iter, factor);

    double elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("elapsed time: %.8f seconds\n", elapsed_time);

    return 0;
}
