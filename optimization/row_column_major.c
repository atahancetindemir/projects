#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define ROW 20000
#define COLUMN 20000

unsigned int mat[ROW][COLUMN];

int main() {

    srand(23);
    long long unsigned int sum = 0;
    
    // Fill the matrix
    for(unsigned int i=0; i<ROW; i++) {
        for(unsigned int j=0; j<COLUMN; j++) {
            mat[i][j] = rand() % 100;
        }
    }

    clock_t start = clock();

#ifdef ROW_MAJOR
    // Acces the memory via row major order
    for(unsigned int i=0; i<ROW; i++) {
        for(unsigned int j=0; j<COLUMN; j++) {
            sum += mat[i][j];
        }
    }
#endif


#ifdef COLUMN_MAJOR
    // Acces the memory via column major order
    for(unsigned int j=0; j<COLUMN; j++) {
        for(unsigned int i=0; i<ROW; i++) {
            sum += mat[i][j];
        }
    }
#endif

    clock_t end = clock();
    
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Sum: %llu\n", sum);
    printf("Time taken: %f seconds\n", time_taken);

    return 0;
}
