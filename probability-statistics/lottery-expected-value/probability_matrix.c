#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


// 0 to n exclusive
unsigned int generate_rand(unsigned int n) {
    return rand() % n;
}


// Iterative generator for k-combinations of 1..n in lexicographic order
void combinations_iter(unsigned int n, unsigned int r) {
    if (r > n || r == 0) return;
    unsigned int a[64];                 // r <= 64 for safety here; grow if needed
    for (unsigned int i = 0; i < r; ++i) a[i] = i + 1;

    for (;;) {
        // print current combination
        for (unsigned int i = 0; i < r; ++i) {
            printf("%u%s", a[i], (i + 1 == r ? "\n" : " "));
        }
        // generate next
        int t = (int)r - 1;
        while (t >= 0 && a[t] == n - (r - 1 - t)) --t;   // find rightmost incrementable
        if (t < 0) break;                                // last combo reached
        ++a[t];
        for (unsigned int i = t + 1; i < r; ++i) a[i] = a[i - 1] + 1;
    }
}


// Safe-ish binomial for moderate n (fits into unsigned long long)
// use p_orderstat for large n, r
unsigned long long ncr(unsigned int n, unsigned int r) {
    if (r > n) return 0;
    if (r > n - r) r = n - r; // symmetry
    unsigned long long res = 1;
    for (unsigned int k = 1; k <= r; ++k) {
        // Multiply before divide; use gcd in production if you worry about overflow.
        res = res * (n - r + k) / k;
    }
    return res;
}


// log(C(n, k)) = log(n!) - log(k!) - log((n-k)!)
static inline long double logC(unsigned n, unsigned k) {
    if (k > n) return -INFINITY;
    return lgammal((long double)n + 1.0L)
         - lgammal((long double)k + 1.0L)
         - lgammal((long double)(n - k) + 1.0L);
}


// log(P(X=k)) = log(C(k-1, j-1)) + log(C(n-k, r-j)) - log(C(n, r))
static double p_orderstat(unsigned n, unsigned r, unsigned k, unsigned j) {
    if (j<1 || j>r) return 0.0;
    if (k<j || k>n-(r-j)) return 0.0;   // zeros
    long double lp = logC(k-1, j-1) + logC(n-k, r-j) - logC(n, r);
    long double p  = expl(lp);
    // if (p < 0) p = 0; if (p > 1) p = 1;  // clamp
    return (double)p;
}


// Sanity check: sum of each column should be ~1
static void sanity_check(unsigned n, unsigned r) {
    for (unsigned j = 1; j <= r; ++j) {
        long double s = 0.0L;
        for (unsigned i = 1; i <= n; ++i) s += p_orderstat(n, r, i, j);
        // Should be ~1 with tiny numerical error
        printf("sum column k%-2u = %.18Lf\n", j, s);
    }
}


// Write probability matrix to CSV file
static void write_prob_csv(unsigned n, unsigned r, float mat[n][r], const char* path) {
    FILE* f = fopen(path, "w");
    if (!f) { perror("fopen"); exit(1); }
    // header
    fprintf(f, "i\\k");
    for (unsigned j = 1; j <= r; ++j) fprintf(f, ",k%u", j);
    fputc('\n', f);

    for (unsigned i = 1; i <= n; ++i) {
        fprintf(f, "%u", i);
        for (unsigned j = 1; j <= r; ++j) {
            long double p = mat[i-1][j-1];
            // scientific notation with high precision
            fprintf(f, ",%.18Le", p);
        }
        fputc('\n', f);
    }
    fclose(f);
}


// Display array
void arr_display(unsigned int size, unsigned int arr[size]) {
    for (int i = 0; i < size; i++) {
        printf("%u ", arr[i]);
    }
}


// Zero out arr
void arr_zero(unsigned int size, unsigned int arr[size]) {
    for (unsigned int i = 0; i < size; i++) {
        arr[i] = 0;
    }
}


// arr[i] = i+1
void arr_fill(unsigned int size, unsigned int arr[size]) {
    for (unsigned int i = 0; i < size; i++) {
        arr[i] = i+1; 
    }
}


// Delete pos
unsigned int arr_del(unsigned int size, unsigned int pos, unsigned int arr[size]) {
    for(int i = pos; i < size - 1; i++) {
        arr[i] = arr[i+1];
    }
    return size - 1;
}


// Copy src to dest
void arr_cpy(unsigned int size, unsigned int dest[size], unsigned int src[size]) {
    for(unsigned int i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}


// Stable Insertion Sort: ascending
void arr_sort(unsigned int n, unsigned int a[n]) {
    if (n < 2) return;
    for (unsigned int i = 1; i < n; ++i) {
        unsigned int j = i, key = a[i];
        while (j > 0 && a[j - 1] > key) {
            a[j] = a[j - 1];
            --j;
        }
        a[j] = key;
    }
}


// Place C(n,r) of arr into arr_comb
void arr_create_comb(unsigned int n, unsigned int r, unsigned int arr[n], unsigned int comb[r]) {
    unsigned int rand = 0, temp[n];

    // think about creating temp outside of this function

    // don't lose original array
    arr_cpy(n, temp, arr);

    for (unsigned int i = 0; i < r; i++) {
        rand = generate_rand(n);
        comb[i] = temp[rand];
        n = arr_del(n, rand, temp);
    }
}


// Display matrix
void matrix_display(unsigned int row, unsigned int column, float mat[row][column]) {
    for(unsigned int i = 0; i < row; i++) {
        for(unsigned int j = 0; j < column; j++) {
            printf("%.3f ", mat[i][j]);
        }
        printf("\n");
    }
}


// Zero out matrix
void matrix_zero(unsigned int row, unsigned int column, float mat[row][column]) {
    for(unsigned int i = 0; i < row; i++) {
        for(unsigned int j = 0; j < column; j++) {
            mat[i][j] = 0.0;
        }
    }
}


// P(Mij) = p_orderstat(n, r, i, j)
void matrix_large_expected_prob(unsigned int n, unsigned int r, float mat[n][r]) {
    for (unsigned int i = 1; i <= n; i++) {
        for (unsigned int j = 1; j <= r; j++) {
            mat[i-1][j-1] = p_orderstat(n, r, i, j); 
        }
    }
}

// Mij = C(i-1, j-1) * C(n-i, r-j)
void matrix_expected_freq(unsigned int n, unsigned int r, float mat[n][r]) {
    for (unsigned int i = 1; i <= n; i++) {
        for (unsigned int j = 1; j <= r; j++) {
            mat[i-1][j-1] = ncr(i-1, j-1) * ncr(n-i, r-j);
        }
    }
}


// P(Mij) = Mij / C(n, r)
void matrix_expected_prob(unsigned int n, unsigned int r, float mat[n][r]) {

    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j < r; j++) {
            mat[i][j] /= ncr(n, r);
        }
    }
}


// P(Mij) = Mij / t
void matrix_calc_prob(unsigned int n, unsigned int r, unsigned int t, float mat[n][r]) {
    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j < r; j++) {
            mat[i][j] /= t;
        }
    }
}


// Mij += 1 for each occurrence of value i at position j in comb
void matrix_accumulate(unsigned int n, unsigned int r, unsigned int comb[r], float mat[n][r]) {
    for (unsigned int j = 0; j < r; ++j) {
        unsigned int v = comb[j];          // value in {1..n}
        if (v >= 1 && v <= n) {
            mat[v - 1][j] += 1.0f;         // row = value-1, col = position j
        }
    }
}


int main() {
    srand(52);

    unsigned int n = 49, r = 6, t = 100;
    
    float matA[n][r], matB[n][r];
    unsigned int arr[n];
    unsigned int comb[r];

    // combinations_iter(n, r);

    printf("\n");
    arr_fill(n, arr);
    matrix_zero(n, r, matA);    
    
    for(unsigned int i = 0; i < t; i++) {
        arr_create_comb(n, r, arr, comb);
        arr_sort(r, comb);
        matrix_accumulate(n, r, comb, matA);
    }
    matrix_calc_prob(n , r, t, matA);
    matrix_display(n, r, matA);

    printf("\n\n");

    matrix_large_expected_prob(n, r, matB);
    matrix_display(n, r, matB);

    // printf("\n");
    // write_prob_csv(n, r, matA, "prob_matrix.csv");
    // sanity_check(n, r);


    return 0;
}
