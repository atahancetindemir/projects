#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 3
#define RANDOM_MAX 50

void reset_matrix(double mat[SIZE][SIZE]) {
    for(unsigned int i=0 ; i<SIZE; i++) {
        for(unsigned int j=0; j<SIZE; j++) {
            mat[i][j] = 0;
        }
    }
}

double find_determinant(double mat[SIZE][SIZE]) {
    return 0;
}

void invert_matrix(double mat[SIZE][SIZE]) {
    return;
}

double sum_of_diagonals(double mat[SIZE][SIZE]) {
    double sum = 0;

    for(unsigned int i=0; i<SIZE; i++) {
        for(unsigned int j=0; j<SIZE; j++) {
            if(i == j) {
                sum += mat[i][j];
            }
        }
    }
    return sum;
}

double sum_of_triangulars(double mat[SIZE][SIZE], char operation) {
    double sum = 0;

    for(unsigned int i=0; i<SIZE; i++) {
        for(unsigned int j=0; j<SIZE; j++) {
            if(operation == 'u') {
                if(j >= i) {
                    sum += mat[i][j];
                }
            }
            else if(operation == 'd') {
                if(i >= j) {
                    sum += mat[i][j];
                }
            }
            else {
                printf("invalid operation");
                exit(-1);
            }
        }
    }
    return sum; 
}

void transpose_matrix(double mat[SIZE][SIZE]) {
    double temp[SIZE][SIZE];

    for(unsigned int i=0; i<SIZE; i++) {
        for(unsigned int j=0; j<SIZE; j++) {
            temp[i][j] = mat[i][j];
        }
    }
    for(unsigned int i=0; i<SIZE; i++) {
        for(unsigned int j=0; j<SIZE; j++) {
            mat[i][j] = temp[j][i];
        }
    }
}

void scalar_matrix(double mat[SIZE][SIZE], double constant, char operation) {
    for(unsigned int i=0; i<SIZE; i++) {
        for(unsigned int j=0; j<SIZE; j++) {
            if(operation == '+') {
                mat[i][j] += constant;
            }
            else if(operation == '-') {
                mat[i][j] -= constant;
            }
            else if(operation == '*') {
                mat[i][j] *= constant;
            }
            else if(operation == '/') {
                mat[i][j] /= constant;
            }
            else {
                printf("invalid input");
                exit(-1);
            }
        }
    }
}

void multiply_matrix(double matA[SIZE][SIZE], double matB[SIZE][SIZE], double matC[SIZE][SIZE]) {
    for(unsigned int i=0; i<SIZE; i++) {
        for(unsigned int j=0; j<SIZE; j++) {
            matC[i][j] = 0;
            for(unsigned int k=0; k<SIZE; k++) {
                matC[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
}

void create_matrix(double mat[SIZE][SIZE]) {
    for(unsigned int i=0; i<SIZE; i++) {
        for(unsigned int j=0; j<SIZE; j++) {
            mat[i][j] = (rand() % RANDOM_MAX) + 1;
        }
    }
}

void display_matrix(double mat[SIZE][SIZE]) {
    for(unsigned int i=0; i<SIZE; i++) {
        for(unsigned int j=0; j<SIZE; j++) {
            printf("%.2lf\t", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void sum_matrix(double matA[SIZE][SIZE], double matB[SIZE][SIZE], double matC[SIZE][SIZE]) {
    for(unsigned int i=0; i<SIZE; i++) {
        for(unsigned int j=0; j<SIZE; j++) {
            matC[i][j] = matA[i][j] + matB[i][j];
        }
    }
}

void substract_matrix(double matA[SIZE][SIZE], double matB[SIZE][SIZE], double matC[SIZE][SIZE]) {
    for(unsigned int i=0; i<SIZE; i++) {
        for(unsigned int j=0; j<SIZE; j++) {
            matC[i][j] = matA[i][j] - matB[i][j];
        }
    }
}

int main() {
    double matA[SIZE][SIZE];
    double matB[SIZE][SIZE];
    double matC[SIZE][SIZE];

    srand(time(NULL));

    create_matrix(matA);
    create_matrix(matB);
    reset_matrix(matC);

    display_matrix(matA);
    display_matrix(matB);
    display_matrix(matC);

    substract_matrix(matA, matB, matC);

    display_matrix(matC);
    
    return 0;
}
