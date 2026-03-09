#include <stdio.h>
#include <stdint.h>
#include <time.h>

uint32_t calculate_nth_fibonacci(uint32_t n) {
    uint32_t first = 1;
    uint32_t second = first;
    uint32_t next = 0;

    for (uint32_t i=0; i<n; i++) {
        next = first + second;
        first = second;
        second = next;
    }
    return next;
}

int main(int argc, char* argv[]) {
    uint32_t n = 0;

    if(argc == 2) {
        sscanf(argv[1], "%u", &n);
    }
    else {
        return 1;
    }

    clock_t start = clock();
    uint32_t fib = calculate_nth_fibonacci(n);
    clock_t end = clock();

    double elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%lf\n", elapsed_time);
    printf("%u\n", fib);
    
    return 0;
}