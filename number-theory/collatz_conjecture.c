#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define STEP_LIMIT 100000 // Trigger for exit in the infinite loop
#define MAX_DIGITS 1000 // Max block size used while creating big numbers

// Displays the big number stored in a reverse-ordered array
void display_number(uint8_t num[], uint16_t size) {
    for (int16_t i = size - 1; i >= 0; i--) {
        printf("%u", num[i]);
    }
    printf("\n");
}

// Adds a scalar to a big number stored in a reverse-ordered array
void add_scalar(uint8_t num[], uint16_t* size, uint16_t scalar) {
    uint16_t carry = scalar;
    uint16_t max_index = 0; // Track the highest index updated

    for (uint16_t i = 0; i < *size || carry; i++) {
        uint8_t digit = (i < *size) ? num[i] : 0;
        uint8_t sum = digit + carry;
        num[i] = sum % 10;
        carry = sum / 10;
        max_index = i; // Update the highest index processed
    }

    // Update size to reflect the highest index + 1
    if (max_index >= *size) {
        *size = max_index + 1;
    }
}

// Multiplies a big number by a scalar, stored in a reverse-ordered array
void multiply_scalar(uint8_t num[], uint16_t* size, uint16_t scalar) {
    uint16_t carry = 0;

    for (uint16_t i = 0; i < *size || carry; i++) {
        if (i == *size) { 
            num[i] = 0;  // If i goes outside the current sequence, we add a new digit.
            (*size)++;   // We increase the size here because we added a new digit due to carry.
        }

        uint16_t product = num[i] * scalar + carry;
        num[i] = product % 10;
        carry = product / 10;
    }
}


// Divides a big number by a scalar, stored in a reverse-ordered array
void divide_scalar(uint8_t num[], uint16_t* size, uint16_t scalar) {
    uint16_t remainder = 0;
    
    for (int16_t i = *size - 1; i >= 0; i--) {
        uint32_t current = remainder * 10 + num[i];
        num[i] = current / scalar;
        remainder = current % scalar;
    }
    
    // Remove unnecessary leading zeros, but leave at least 1 digit.
    while (*size > 1 && num[*size - 1] == 0) {
        (*size)--;
    }
    
    // If the number resulting from division is exactly zero, leave at least '0'
    // if (*size == 0) {
    //     *size = 1;
    //     num[0] = 0;
    // }
}

// Produces a big number that is a power of 2, stored in a reverse-ordered array
uint8_t* create_number(uint16_t* size, uint16_t exp, uint16_t block_size) {
    *size = 1;
    // Potential buffer overflow when block_size (aka MAX_DIGITS) is not enough large to store all digits
    uint8_t* num = (uint8_t*)calloc(block_size, sizeof(uint8_t)); // Allocate a large enough array
    num[0] = 1; // Initial value (2^0)
    
    for (uint16_t i = 0; i < exp; i++) {
        uint32_t carry = 0;
        for (uint16_t j = 0; j < *size || carry; j++) {
            if (j >= block_size) break;

            uint32_t product = (j < *size ? num[j] : 0) * 2 + carry;
            num[j] = product % 10;
            carry = product / 10;

            if (j >= *size) (*size)++;
        }
    }
    return num;
}

/* Compare a and b, stored in a reverse-ordered array.
   Returns -1 if a < b, returns 0 if a = b, returns 1 if a > b*/
int compare_numbers(uint8_t* a, uint16_t size_a, uint8_t* b, uint16_t size_b) {
    if (size_a > size_b) return 1;
    if (size_a < size_b) return -1;
    for (int i = size_a - 1; i >= 0; i--) {
        if (a[i] > b[i]) return 1;
        if (a[i] < b[i]) return -1;
    }
    return 0;
}

// Computes the Collatz sequence for very big numbers within a given range.
void collatz_large(uint8_t* lower, uint16_t size_lower, uint8_t* upper, uint16_t size_upper, uint16_t block_size) {
    uint64_t iter_total = 0;
    bool flag_break = false;
    uint8_t temp[block_size];
    uint16_t size_temp;

    while(compare_numbers(lower, size_lower, upper, size_upper) <= 0) {
        memcpy(temp, lower, size_lower);
        size_temp = size_lower;
        uint64_t iter_per_sample = 0;
        
        while (size_temp != 1 || temp[0] != 1) {
            // display_number(temp, size_temp);

            if (temp[0] % 2 == 0) { // Even
                divide_scalar(temp, &size_temp, 2);
            }
            else {                  // Odd
                multiply_scalar(temp, &size_temp, 3);
                add_scalar(temp, &size_temp, 1);
            }

            iter_per_sample++;
            if (iter_per_sample > STEP_LIMIT) {
                printf("\niteration exceeded limit\nnumber: ");
                display_number(lower, size_lower);
                printf("\n");
                flag_break = true;
                break;
            }
        }
        iter_total += iter_per_sample;
        // printf("\n");
        if (flag_break) break;
        add_scalar(lower, &size_lower, 1);
    }
}

int main() {
    uint8_t *num1, *num2 = 0;
    uint16_t size1, size2 = 0;

    num1 = create_number(&size1, 1, MAX_DIGITS);
    num2 = create_number(&size2, 23, MAX_DIGITS);

    collatz_large(num1, size1, num2, size2, MAX_DIGITS);

    free(num1);
    free(num2);


    return 0;
}
