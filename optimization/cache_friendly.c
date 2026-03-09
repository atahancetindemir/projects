#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 4096
#define BLOCK 64

void mat_mul_cache_friendly(int size, int block, int** A, int** B, int** C) {

    // Generally i-k-j is better for cache locality.
    for(int ii=0; ii<size; ii+=block) {
        for(int kk=0; kk<size; kk+=block) {
            for(int jj=0; jj<size; jj+=block) {

                // Avoid overflow
                int i_max = (ii + block < size) ? ii + block : size;
                int k_max = (kk + block < size) ? kk + block : size;
                int j_max = (jj + block < size) ? jj + block : size;

                for(int i=ii; i<i_max; i++) {
                    for(int k=kk; k<k_max; k++) {
                        // Encourage the use of register
                        int temp_a = A[i][k];
                        for(int j=jj; j<j_max; j++) {
                            C[i][j] += temp_a * B[k][j];
                        }
                    }
                }
            }
        }
    }
}

void mat_display(int size, int** mat) {
    for(int i=0; i<size; i++) {
        for(int j=0; j<size; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}

void mat_init_rand(int size, int** mat) {
    for(int i=0; i<size; i++) {
        for(int j=0; j<size; j++) {
            mat[i][j] = rand() % 100;
        }
    }
}

int** mat_allocate(int size) {
    int** mat = (int**)malloc(size * sizeof(int*));
    for(int i=0; i<size; i++) {
        mat[i] = (int*)malloc(size * sizeof(int));
    }
    return mat;
}

void mat_mul_naive(int size, int** A, int** B, int** C) {
    for(int i=0; i<size; i++) {
        for(int j=0; j<size; j++) {
            C[i][j] = 0;
            for(int k=0; k<size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int** A, **B, **C;

    A = mat_allocate(SIZE);
    B = mat_allocate(SIZE);
    C = mat_allocate(SIZE);

    srand(58);

    mat_init_rand(SIZE, A);
    mat_init_rand(SIZE, B);

    clock_t start = clock();
#ifdef NAIVE
    mat_mul_naive(SIZE, A, B, C);
#endif

#ifdef CACHE_FRIENDLY
    mat_mul_cache_friendly(SIZE, BLOCK, A, B, C);
#endif
    clock_t end = clock();

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f sec\n", time_taken);

    // mat_display(SIZE, C);

    free(A);
    free(B);
    free(C);

    return 0;
}

/*

gcc cache_friendly.c -o cache_friendly -DNAIVE
./cache_friendly 
Time taken: 464.010846 sec

gcc cache_friendly.c -o cache_friendly -DCACHE_FRIENDLY
./cache_friendly 
Time taken: 134.612421 sec

*/