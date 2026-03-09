#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000000
#define REPEAT 10000

// To accurately measure the effect of loop unrolling, use these flags:
// -O2 -fno-unroll-loops -fno-tree-vectorize

// using -O0 instead of -O2 will cause the variables s1, s2, s3, s4 stored in memory instead of registers,
// not using fno-tree-vectorize may cause the compiler to auto-vectorize the loop,
// not using fno-unroll-loops may cause the compiler to auto-unroll the loop,

int main() {

    srand(49);

    int* arr = malloc(sizeof(int) * SIZE);
    if(!arr) return 1;
    
    // Avoid constant folding by randomizing array data
    for(int i=0; i<SIZE; i++) arr[i] = rand() % 10;

    long long unsigned int s1 = 0, s2 = 0, s3 = 0, s4 = 0;
    long long unsigned int sum = 0;
    
    clock_t start = clock();

#ifdef NO_UNROLL
    for(int k=0; k<REPEAT; k++) {
        for(int i=0; i<SIZE; i++) {
            sum += arr[i]; // Some arbitrary computation
        }
    }
#endif

#ifdef UNROLL_4
    for(int k=0; k<REPEAT; k++) {
        int i;
        for(i=0; i<SIZE-4; i+=4) {
            sum += arr[i];
            sum += arr[i+1];
            sum += arr[i+2];
            sum += arr[i+3];
        }
        // Remainder loop
        for(; i<SIZE; i++) sum += arr[i];
    }
#endif   

#ifdef UNROLL_4_PARALLEL
    for(int k=0; k<REPEAT; k++) {
        int i;
        for(i=0; i<SIZE-4; i+=4) {
            // Superscalar Execution
            // Dependency Chain Break
            s1 += arr[i];
            s2 += arr[i+1];
            s3 += arr[i+2];
            s4 += arr[i+3];
        }
        // Remainder loop
        for(; i<SIZE; i++) s1 += arr[i];
        sum = s1 + s2 + s3 + s4;
    }
#endif    

#ifdef UNROLL_16
    // Register Spilling may occur here due to high register pressure
    for(int k=0; k<REPEAT; k++) {
        int i;
        for(i=0; i<SIZE-16; i+=16) {
            sum += arr[i] + arr[i+1] + arr[i+2] + arr[i+3] +
                arr[i+4] + arr[i+5] + arr[i+6] + arr[i+7] +
                arr[i+8] + arr[i+9] + arr[i+10] + arr[i+11] +
                arr[i+12] + arr[i+13] + arr[i+14] + arr[i+15];
            
        }
        // Remainder loop
        for(; i<SIZE; i++) sum += arr[i];
    }
#endif

    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Sum: %llu\n", sum);
    printf("Time taken: %f sec\n", time_taken);

    free(arr);
    return 0;
}

/*

gcc loop_unrolling.c -O2 -fno-unroll-loops -fno-tree-vectorize -o loop_unrolling -DNO_UNROLL
Sum: 45036370000
Time taken: 2.451090 sec
For each addition operation, the processor does the following: increment, check if i < SIZE, jump back to the beginning.


gcc loop_unrolling.c -O2 -fno-unroll-loops -fno-tree-vectorize -o loop_unrolling -DUNROLL_4
Sum: 45036370000
Time taken: 1.195453 sec
Processor does this every four addition operation.


gcc loop_unrolling.c -O2 -fno-unroll-loops -fno-tree-vectorize -o loop_unrolling -DUNROLL_4_PARALLEL
Sum: 45036370000
Time taken: 1.198986 sec
No significant improvement over standard unroll_4 (Unexpected theoretically).
Since integer addition is a very cheap operation (1 clock cycle), the bottleneck 
here is Memory Latency (waiting for data from L1 Cache), not the CPU execution unit.
Breaking the dependency chain (s1...s4) doesn't help because the CPU is 
waiting for data, not for the previous calculation.

You can try again with a more costly operation (for example, division).


gcc loop_unrolling.c -O2 -fno-unroll-loops -fno-tree-vectorize -o loop_unrolling -DUNROLL_16
Sum: 45036370000
Time taken: 1.111946 sec 
Loop overhead is minimized (1 check per 16 elements).
Despite the large block, Register Spilling did not occur significantly.
The long expression allowed the compiler/CPU to interleave memory loads and 
arithmetic operations optimally, hiding some memory latency.

You can try with a larger block to observe register spill.

*/

