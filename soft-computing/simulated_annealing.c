#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

float metropolis(float df, float k, float temp) {
    return expf(-df / (k * temp));
}

float urand01(void) {
    return (float)rand() / ((float)RAND_MAX + 1.0f);
}

float calculate_df(float next, float cur) {
    return next - cur;
}

float fitness(float x1, float x2) {
    return 500 - 20*x1 - 26*x2 - 4*x1*x2 + 4*x1*x1 + 3*x2*x2;
}

float calculate_range(float x, float interval) {
    float lower= x - interval;
    float upper= x + interval;
    float rand = urand01();
    return lower + rand * (upper - lower);
}

void setup(float temp, float c, float k, float x1, float x2, float interval) {
    printf("Simulation Parameters:\n");
    printf("Temperature:\t%f\n", temp);
    printf("Reductor (c):\t%f\n", c);
    printf("Boltzmann (k):\t%f\n", k);
    printf("Initial x1:\t%f\n", x1);
    printf("Initial x2:\t%f\n", x2);
    printf("Interval:\t%f\n\n", interval);
}

void display_worse(unsigned idx, float r1, float r2, float f, float f_new, float f_best, float df, float rand, float prob) {
    printf("idx:\t%u\nr1:\t%f\nr2:\t%f\nf:\t%f\nf_new:\t%f\nf_best:\t%f\ndf:\t%f\nrand:\t%f\nprob:\t%f\n\n", idx, r1, r2, f, f_new, f_best, df, rand, prob);
}

void display_better(unsigned idx, float r1, float r2, float f, float f_new, float f_best, float df) {
    printf("idx:\t%u\nr1:\t%f\nr2:\t%f\nf:\t%f\nf_new:\t%f\nf_best:\t%f\ndf:\t%f\n\n", idx, r1, r2, f, f_new, f_best, df);
}

void display_intermediate(unsigned iter, float temp) {
    printf("Iter\t%d\nTemp\t%f\n\n", iter+1, temp);
}

// TODO:
// Add different objective functions

int main() {
    srand(34);

    float temp = 384.25, c = 0.9, k_boltzmann = 1, x1 = 4.0f, x2 = 5.0f, interval = 6.0f; 
    float r1 = 0, r2 = 0, f = 0, f_new = 0, df = 0, rand = 0, prob = 0;
    float f_best = fitness(x1, x2);
    unsigned n = 3, iter = 5;
    

    setup(temp, c, k_boltzmann, x1, x2, interval);

    // Minimization
    for(unsigned i=0; i<iter; i++) {
        display_intermediate(i, temp);
        for(unsigned j=0; j<n; j++) {
            r1 = calculate_range(x1, interval);
            r2 = calculate_range(x2, interval);
            f = fitness(x1, x2);
            f_new = fitness(r1, r2);
            df = calculate_df(f_new, f);

            char accepted = 0;
            if(df <= 0.0f) {
                // better solution -> unconditional acceptance
                accepted = 1;
                x1 = r1;
                x2 = r2;
                display_better(j+1, r1, r2, f, f_new, f_best, df);
            }
            else {
                // worse solution -> conditional acceptance
                rand = urand01();
                prob = metropolis(df, k_boltzmann, temp);
                display_worse(j+1, r1, r2, f, f_new, f_best, df, rand, prob);

                if(prob > rand) {
                    // accept worse solution
                    x1 = r1;
                    x2 = r2;
                    accepted = 1;
                }
                    // reject worse solution
            }
            // update best solution found so far
            if(accepted && (f_new < f_best))
                f_best = f_new;
        }
        temp *= c;
    }
    
    
    return 0;
}