#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define BINARY 2

// 0 to n exclusive
unsigned generate_rand(unsigned n) {
    return rand() % n;
}


float generate_rand01(void) {
    return (float)generate_rand(100001) / 100001.0f;
}


// Copy src to dest
void arr_cpy(unsigned size, float* dest, float* src) {
    for(unsigned i=0; i<size; i++) {
        dest[i] = src[i];
    }
}


void arr_fill(unsigned size, float* arr, float value) {
    for(unsigned i=0; i<size; i++) {
        arr[i] = value;
    }
}


void arr_zero(unsigned size, float* arr) {
    for(unsigned i=0; i<size; i++) {
        arr[i] = 0.0f;
    }
}


float arr_sum(unsigned size, float* arr) {
    unsigned sum = 0.0f;
    for(unsigned i=0; i<size; i++) {
        sum += arr[i];
    }
    return sum;
}


float arr_max(unsigned size, float* arr) {
    float max = arr[0];
    for(unsigned i=0; i<size; i++) {
        if(arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}


float arr_min(unsigned size, float* arr) {
    float min = arr[0];
    for(unsigned i=0; i<size; i++) {
        if(arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}

void arr_display_float(unsigned size, float* arr) {
    for(unsigned i=0; i<size; i++) {
        printf("%f ", arr[i]);
    }
}

void arr_display_uint(unsigned size, unsigned* arr) {
    for(unsigned i=0; i<size; i++) {
        printf("%u ", arr[i]);
    }
}


void mat_cpy(unsigned row, unsigned column, unsigned dest[row][column], unsigned src[row][column]) {
    for(unsigned i=0; i<row; i++) {
        for(unsigned j=0; j<column; j++) {
            dest[i][j] = src[i][j];
        }
    }
}


void create_chromosome(unsigned pop, unsigned gen, unsigned chr_bin[pop][gen]) {
    for(unsigned i=0; i<pop; i++) {
        for(unsigned j=0; j<gen; j++) {
            chr_bin[i][j] = generate_rand(BINARY);
        }
    }
}


float max_weight(float fitness_x) {
    return fitness_x;
}


float min_weight(float fitness_x) {
    return 1.0f / (1.0f + fitness_x);
}


void normalize_prob(unsigned pop, float* chr_weight) {
    float sum = 0.0f;
    for(unsigned i=0; i<pop; i++) {
        sum += chr_weight[i];
    }
    for(unsigned i=0; i<pop; i++) {
        chr_weight[i] /= sum;
    }
}


void roulette_wheel_selection(unsigned pop, float* chr_cum, unsigned* parent, float* rand) {
    for(unsigned i=0; i<pop; i++) {
        rand[i] = generate_rand01();
        for(unsigned j=0; j<pop; j++) {
            if(rand[i] <= chr_cum[j]) {
                // Same parent can be selected multiple times (With Replacement)
                // This is simulating the selection pressure towards fitter chromosomes
                // Think about animals fighting for the females, the stronger ones have higher chances of mating
                parent[i] = j;
                break;
            }
        }
    }
}


void cumulative_prob(unsigned pop, float* chr_weight, float* chr_cum) {
    chr_cum[0] = chr_weight[0];
    for(unsigned i=1; i<pop; i++) {
        chr_cum[i] = chr_cum[i-1] + chr_weight[i];
    }
}


void crossover_one_point(unsigned pop, unsigned gen, unsigned chr_bin[pop][gen], unsigned* parent, float p_crossover) {
    unsigned chr_temp[pop][gen];
    mat_cpy(pop, gen, chr_temp, chr_bin);
    
    for(unsigned i=0; i<pop; i+=2) {
        float rand = generate_rand01();
        if(rand <= p_crossover) {
            unsigned point = generate_rand(gen-1) + 1;
            // printf("Crossover occurred between Chromosome %u and Chromosome %u at Point %u\n", parent[i]+1, parent[i+1]+1, point);
            for(unsigned j=0; j<gen; j++) {
                if(j < point) {
                    chr_bin[i][j] = chr_temp[parent[i]][j];
                    chr_bin[i+1][j] = chr_temp[parent[i+1]][j];
                } else {
                    chr_bin[i][j] = chr_temp[parent[i+1]][j];
                    chr_bin[i+1][j] = chr_temp[parent[i]][j];
                }
            }
        } else {
            // No Crossover, Direct Copy
            for(unsigned j=0; j<gen; j++) {
                chr_bin[i][j] = chr_temp[parent[i]][j];
                chr_bin[i+1][j] = chr_temp[parent[i+1]][j];
            }
        } 
    }
}                                                               


void mutation(unsigned pop, unsigned gen, unsigned chr_bin[pop][gen], float p_mutation) {
    for(unsigned i=0; i<pop; i++) {
        for(unsigned j=0; j<gen; j++) {
            float rand = generate_rand01();
            if(rand <= p_mutation) {
                // printf("Mutation occurred at Chromosome %u Gene %u\n", i+1, j+1);
                chr_bin[i][j] ^= 1u;
            }
        }
    }
}

float fitness(float x) {
    return 255*x - x*x;
}


void convert_dec(unsigned pop, unsigned gen, unsigned chr_bin[pop][gen], float* chr_dec) {
    for(unsigned i=0; i<pop; i++) {
        unsigned dec_val = 0;
        for(unsigned j=0; j<gen; j++) {
            dec_val = (dec_val << 1) | chr_bin[i][j];
        }
        chr_dec[i] = (float)dec_val;
    }
}


void setup(unsigned pop, unsigned gen, unsigned t, float p_crossover, float p_mutation) {
    printf("Simulation Parameters\n");
    printf("Population: %u\n", pop);
    printf("Genome Count: %u\n", gen);
    printf("Iteration Count: %u\n", t);
    printf("Crossover Probability: %f\n", p_crossover);
    printf("Mutation Probability: %f\n", p_mutation);
}


void info(unsigned pop, unsigned* parent, float* rand) {
    printf("\nGenerated Random Values: ");
    arr_display_float(pop, rand);
    printf("\nSelected Parents: ");
    arr_display_uint(pop, parent);
    printf("\n\n");
}


void display(unsigned pop, unsigned gen, unsigned chr_bin[pop][gen], float* chr_dec, float* chr_fit,
     float* chr_weight, float* chr_cum, float* avg, float* best, unsigned idx) {

    unsigned col_width = (gen > 20) ? gen : 20;

    printf("\nGeneration %u\n", idx);
    printf("\n%-20s %-*s %-20s %-20s %-20s %-20s\n", "Chromosome", col_width, "Binary", "Decimal", "Fitness", "Probability", "Cumulative");

    for(unsigned i=0; i<pop; i++) {
        printf("%-20u ", i+1);
        for(unsigned j=0; j<gen; j++) {
            printf("%u", chr_bin[i][j]);
        }
        int padding = col_width - gen;
        if(padding < 0) padding = 0;
        printf("%-*s", padding, "");
        printf(" ");
        printf("%-20f %-20f %-20f %-20f\n", chr_dec[i], chr_fit[i], chr_weight[i], chr_cum[i]);
    }
        printf("\nAverage: %f\nBest: %f\n\n", avg[idx], best[idx]);
}


void compute_avg_best(unsigned pop, float* chr_fit, float* avg, float* best, float* temp, unsigned idx, char obj_func) {

    if(!obj_func) { // Max
        float max = arr_max(pop, chr_fit);
        if(max > *temp) *temp = max;
    }
    else {          // Min
        float min = arr_min(pop, chr_fit);
        if(min < *temp) *temp = min;
    }
    
    best[idx] = *temp;

    avg[idx] = arr_sum(pop, chr_fit);
    avg[idx] /= (float)pop;
}


void evaluate(unsigned pop, unsigned gen, unsigned chr_bin[pop][gen], float* chr_dec, float* chr_fit,
     float* chr_weight, float* chr_cum, float* avg, float* best, unsigned idx, float* temp, char obj_func) {

    // Compute everything for the current pop

    convert_dec(pop, gen, chr_bin, chr_dec);
    arr_cpy(pop, chr_fit, chr_dec);

    for(unsigned i=0; i<pop; i++)
        chr_fit[i] = fitness(chr_fit[i]);

    compute_avg_best(pop, chr_fit, avg, best, temp, idx, obj_func);

    for(unsigned i=0; i<pop; i++)
        chr_weight[i] = (!obj_func) ? max_weight(chr_fit[i]) : min_weight(chr_fit[i]);

    normalize_prob(pop, chr_weight);
    cumulative_prob(pop, chr_weight, chr_cum);

}


void create_csv(unsigned size, float* avg, float* best) {
    FILE* file = fopen("output.csv", "w");
    if(file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "Generation,Average,Best\n");
    for(unsigned i=0; i<size; i++) {
        fprintf(file, "%u,%f,%f\n", i+1, avg[i], best[i]);
    }

    fclose(file);
}


int main() {

    srand(113);
    
    char obj_func = 0; // 0: Max, 1: Min
    unsigned pop = 20;
    unsigned gen = 8;
    unsigned iter = 100;
    float temp = (!obj_func) ? 0.0F : 100000000.0F;
    float p_crossover = 0.70F;
    float p_mutation = 0.001F;

    unsigned chr_bin[pop][gen];
    unsigned parent[pop];
    float chr_dec[pop];
    float chr_fit[pop];
    float chr_weight[pop];
    float chr_cum[pop];
    float rand[pop];
    float best[iter];
    float avg[iter];

    // setup(pop, gen, iter, p_crossover, p_mutation);
    create_chromosome(pop, gen, chr_bin);
    evaluate(pop, gen, chr_bin, chr_dec, chr_fit, chr_weight, chr_cum, avg, best, 0, &temp, obj_func);
    // display(pop, gen, chr_bin, chr_dec, chr_fit, chr_weight, chr_cum, avg, best, 0);

    for(unsigned i=1; i<iter; i++) {
        roulette_wheel_selection(pop, chr_cum, parent, rand);
        // info(pop, parent, rand);
        crossover_one_point(pop, gen, chr_bin, parent, p_crossover);
        mutation(pop, gen, chr_bin, p_mutation);
        evaluate(pop, gen, chr_bin, chr_dec, chr_fit, chr_weight, chr_cum, avg, best, i, &temp, obj_func);
        // display(pop, gen, chr_bin, chr_dec, chr_fit, chr_weight, chr_cum, avg, best, i);
    }

    create_csv(iter, avg, best);

    return 0;
}
