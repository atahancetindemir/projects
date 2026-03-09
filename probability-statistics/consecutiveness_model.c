#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

// Generating PRNG integers between 0 and face_count inclusive.
uint8_t prng(uint8_t face_count) {
    return rand() % face_count;
}

// Returns the average number of iterations by calculating the iterations per sample.
float calculate_average(uint64_t cummulative_iteration, uint32_t total_experiment) {
    return (float)cummulative_iteration / (float)total_experiment;
}

uint32_t calculate_expected(uint8_t face_count, uint8_t consecutive_count) {
    uint32_t sum = 0;
    for(uint8_t i = 1; i <= consecutive_count; i++) {
        sum += pow(face_count, i);
    }
    return sum;
}

uint32_t calculate_expected_second_approach(uint8_t face_count, uint8_t consecutive_count) {
    return (pow(face_count, consecutive_count+1) - face_count) / (face_count-1);
}

// Calculates the average of how many trials it takes for a consecutive event to occur.
uint64_t calculate_consecutiveness(uint8_t face_count, uint8_t consecutive_count, uint32_t total_experiments) {
    uint64_t cummulative_iteration = 0;
    uint8_t current_num = 2, temp_num = 2;

    for (uint32_t n = 0; n < total_experiments; n++) {

        uint32_t i = 0;
        uint8_t j = 0;

        while (1) {
            current_num = prng(face_count);
            if (temp_num == current_num) {
                j++;
            }
            else {
                j = 0;
            }
            temp_num = current_num;
            i++;
            if (j == consecutive_count) {
                break;
            }
        }
        cummulative_iteration += i;
    }

    return cummulative_iteration;
}

int main(int argc, char* argv[]) {
    uint8_t consecutive_count;
    uint8_t face_count;
    uint32_t total_experiment;
    uint64_t cummulative_iteration;

    if (argc == 4) {
        sscanf(argv[1], "%hhu", &face_count);
        sscanf(argv[2], "%hhu", &consecutive_count);
        sscanf(argv[3], "%u", &total_experiment); 
    }
    else {
        printf("usage: %s {face_count} {consecutive_count} {total_experiment}\n{face_count}: 8 bit unsigned int"
        "\n{consecutive_count}: 8 bit unsigned int\n{total_experiment}: 32 bit unsigned int\n", argv[0]);
        return 1;
    }

    srand(time(NULL));

    cummulative_iteration = calculate_consecutiveness(face_count, consecutive_count, total_experiment);
    printf("total iteration: %lu\n", cummulative_iteration);
    printf("average number of shots: %f\n", calculate_average(cummulative_iteration, total_experiment));
    printf("expected value: %u\n", calculate_expected(face_count, consecutive_count));
    printf("expected value second approach: %u\n", calculate_expected_second_approach(face_count, consecutive_count));

    return 0;
}
