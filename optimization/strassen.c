#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <immintrin.h>

// Allocate a 1D array for a matrix
int16_t* mat_allocate(size_t rows, size_t cols) {
    return (int16_t*)malloc(rows * cols * sizeof(int16_t));
}

// Free the allocated matrix
void mat_free(int16_t* matrix) {
    free(matrix);
}

// Get matrix element
int16_t get_element(int16_t* matrix, size_t row, size_t col, size_t n) {
    return matrix[row * n + col];
}

// Set matrix element
void set_element(int16_t* matrix, size_t row, size_t col, int16_t value, size_t n) {
    matrix[row * n + col] = value;
}


// Multiply two matrices using AVX2 intrinsics
void mat_mul_vector(int16_t* A, int16_t* B, int16_t* C, size_t n) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            __m256i sum = _mm256_setzero_si256();
            for (size_t k = 0; k < n; k += 16) {
                __m256i a = _mm256_loadu_si256((__m256i*)&A[i * n + k]);
                __m256i b = _mm256_loadu_si256((__m256i*)&B[k * n + j]);
                sum = _mm256_add_epi16(sum, _mm256_mullo_epi16(a, b));
            }
            int16_t temp[16];
            _mm256_storeu_si256((__m256i*)temp, sum);
            int16_t final_sum = 0;
            for (size_t t = 0; t < 16; t++) {
                final_sum += temp[t];
            }
            C[i * n + j] = final_sum;
        }
    }
}

// Add two matrices
void mat_add(int16_t* A, int16_t* B, int16_t* C, size_t n) {
    for (size_t i = 0; i < n * n; i++) {
        C[i] = A[i] + B[i];
    }
}

// Subtract two matrices
void mat_substract(int16_t* A, int16_t* B, int16_t* C, size_t n) {
    for (size_t i = 0; i < n * n; i++) {
        C[i] = A[i] - B[i];
    }
}

/*
 * Strassen's algorithm for matrix multiplication
 * Uses divide-and-conquer method to multiply matrices recursively
 */
void mat_mul_strassen(int16_t* A, int16_t* B, int16_t* C, size_t n) {
    if (n <= 512) {
        /*
        The higher we keep the threshold value for small-sized matrices, the
        more our heap performance will increase, since the Strassen usage
        rate decreases and the Strassen algorithm is more efficient but consumes
        more memory because it performs more basic (sub, add) operations.
        But after a certain point, the Strassen algorithm will become more
        efficient than standard matrix multiplication because it has a better
        complexity value (~O(N^2.8) vs O(N^3)).
        */
        mat_mul_vector(A, B, C, n);
        return;
    }

    size_t new_size = n / 2;
    size_t size = new_size * new_size;

    // FIXME: You can optimize calloc / free operations

    int16_t* A11 = (int16_t*)calloc(size, sizeof(int16_t));
    int16_t* A12 = (int16_t*)calloc(size, sizeof(int16_t));
    int16_t* A21 = (int16_t*)calloc(size, sizeof(int16_t));
    int16_t* A22 = (int16_t*)calloc(size, sizeof(int16_t));
    int16_t* B11 = (int16_t*)calloc(size, sizeof(int16_t));
    int16_t* B12 = (int16_t*)calloc(size, sizeof(int16_t));
    int16_t* B21 = (int16_t*)calloc(size, sizeof(int16_t));
    int16_t* B22 = (int16_t*)calloc(size, sizeof(int16_t));
    int16_t* C11 = (int16_t*)calloc(size, sizeof(int16_t));
    int16_t* C12 = (int16_t*)calloc(size, sizeof(int16_t));
    int16_t* C21 = (int16_t*)calloc(size, sizeof(int16_t));
    int16_t* C22 = (int16_t*)calloc(size, sizeof(int16_t));

    int16_t* M1 = (int16_t*)calloc(size, sizeof(int16_t));
    int16_t* M2 = (int16_t*)calloc(size, sizeof(int16_t));
    int16_t* M3 = (int16_t*)calloc(size, sizeof(int16_t));
    int16_t* M4 = (int16_t*)calloc(size, sizeof(int16_t));
    int16_t* M5 = (int16_t*)calloc(size, sizeof(int16_t));
    int16_t* M6 = (int16_t*)calloc(size, sizeof(int16_t));
    int16_t* M7 = (int16_t*)calloc(size, sizeof(int16_t));

    int16_t* temp1 = (int16_t*)calloc(size, sizeof(int16_t));
    int16_t* temp2 = (int16_t*)calloc(size, sizeof(int16_t));

    for (size_t i = 0; i < new_size; i++) {
        for (size_t j = 0; j < new_size; j++) {
            size_t index = i * new_size + j;
            A11[index] = get_element(A, i, j, n);
            A12[index] = get_element(A, i, j + new_size, n);
            A21[index] = get_element(A, i + new_size, j, n);
            A22[index] = get_element(A, i + new_size, j + new_size, n);
            B11[index] = get_element(B, i, j, n);
            B12[index] = get_element(B, i, j + new_size, n);
            B21[index] = get_element(B, i + new_size, j, n);
            B22[index] = get_element(B, i + new_size, j + new_size, n);
        }
    }

    mat_add(A11, A22, temp1, new_size);
    mat_add(B11, B22, temp2, new_size);
    mat_mul_strassen(temp1, temp2, M1, new_size);

    mat_add(A21, A22, temp1, new_size);
    mat_mul_strassen(temp1, B11, M2, new_size);

    mat_substract(B12, B22, temp2, new_size);
    mat_mul_strassen(A11, temp2, M3, new_size);

    mat_substract(B21, B11, temp2, new_size);
    mat_mul_strassen(A22, temp2, M4, new_size);

    mat_add(A11, A12, temp1, new_size);
    mat_mul_strassen(temp1, B22, M5, new_size);

    mat_substract(A21, A11, temp1, new_size);
    mat_add(B11, B12, temp2, new_size);
    mat_mul_strassen(temp1, temp2, M6, new_size);

    mat_substract(A12, A22, temp1, new_size);
    mat_add(B21, B22, temp2, new_size);
    mat_mul_strassen(temp1, temp2, M7, new_size);

    mat_add(M1, M4, temp1, new_size);
    mat_substract(temp1, M5, temp2, new_size);
    mat_add(temp2, M7, C11, new_size);

    mat_add(M3, M5, C12, new_size);
    mat_add(M2, M4, C21, new_size);

    mat_add(M1, M3, temp1, new_size);
    mat_substract(temp1, M2, temp2, new_size);
    mat_add(temp2, M6, C22, new_size);

    for (size_t i = 0; i < new_size; i++) {
        for (size_t j = 0; j < new_size; j++) {
            size_t index = i * new_size + j;
            set_element(C, i, j, C11[index], n);
            set_element(C, i, j + new_size, C12[index], n);
            set_element(C, i + new_size, j, C21[index], n);
            set_element(C, i + new_size, j + new_size, C22[index], n);
        }
    }

    // Free allocated memory
    free(A11); free(A12); free(A21); free(A22);
    free(B11); free(B12); free(B21); free(B22);
    free(C11); free(C12); free(C21); free(C22);
    free(M1); free(M2); free(M3); free(M4);
    free(M5); free(M6); free(M7);
    free(temp1); free(temp2);
}

// Check if the number is a power of two
int is_is_power_of_two(size_t n) {
    return (n != 0) && ((n & (n - 1)) == 0);
}

// TODO: Check correctness of the padding for non-square matrices im not sure if it's correct

/**
 * Handle non-square matrices by padding them to the nearest power of two
 * and then performing matrix multiplication using Strassen's algorithm.
 */
void mat_handle_non_square(int16_t* A, int16_t* B, int16_t* C, size_t rowsA, size_t colsA, size_t colsB) {
    size_t max_dim = rowsA > colsA ? rowsA : colsA;
    size_t n = 1;
    while (n < max_dim) {
        n <<= 1;
    }

    int16_t* paddedA = (int16_t*)calloc(n * n, sizeof(int16_t));
    int16_t* paddedB = (int16_t*)calloc(n * n, sizeof(int16_t));
    int16_t* paddedC = (int16_t*)calloc(n * n, sizeof(int16_t));

    for (size_t i = 0; i < rowsA; i++) {
        for (size_t j = 0; j < colsA; j++) {
            paddedA[i * n + j] = get_element(A, i, j, colsA);
        }
    }
    for (size_t i = 0; i < colsA; i++) {
        for (size_t j = 0; j < colsB; j++) {
            paddedB[i * n + j] = get_element(B, i, j, colsB);
        }
    }

    mat_mul_strassen(paddedA, paddedB, paddedC, n);

    for (size_t i = 0; i < rowsA; i++) {
        for (size_t j = 0; j < colsB; j++) {
            set_element(C, i, j, get_element(paddedC, i, j, n), colsB);
        }
    }

    mat_free(paddedA);
    mat_free(paddedB);
    mat_free(paddedC);
}

// Initialize a matrix with random values
void mat_init_rand(int16_t* matrix, size_t rows, size_t cols) {
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            set_element(matrix, i, j, (rand() % 65536) - 32768, cols); // -32,768 to 32,767
        }
    }
}

// Write matrix to a file
void mat_to_file(const char* filename, int16_t* matrix, size_t rows, size_t cols) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Failed to open file %s for writing.\n", filename);
        return;
    }

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            fprintf(file, "%d ", matrix[i * cols + j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

int main() {
    srand(time(NULL));

    int rowsA, colsA, colsB;
    scanf("%d %d %d", &rowsA, &colsA, &colsB);

    // Allocate matrices on heap
    int16_t* A = mat_allocate((size_t)rowsA, (size_t)colsA);
    int16_t* B = mat_allocate((size_t)colsA, (size_t)colsB);
    int16_t* C = mat_allocate((size_t)rowsA, (size_t)colsB);

    // Initialize matrices with random values
    mat_init_rand(A, (size_t)rowsA, (size_t)colsA);
    mat_init_rand(B, (size_t)colsA, (size_t)colsB);

    clock_t start = clock();
    rowsA == colsA && colsA == colsB && is_is_power_of_two((size_t)rowsA) ?
        mat_mul_strassen(A, B, C, (size_t)rowsA) :
        mat_handle_non_square(A, B, C, (size_t)rowsA, (size_t)colsA, (size_t)colsB);
    clock_t end = clock();

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Time taken: %.6f seconds\n", time_taken);

    // Write the resulting matrix to a file.
    mat_to_file("output.txt", C, (size_t)rowsA, (size_t)colsB);

    mat_free(A);
    mat_free(B);
    mat_free(C);

    return 0;
}