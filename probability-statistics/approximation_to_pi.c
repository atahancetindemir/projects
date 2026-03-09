// use monte carlo simulation to approximate pi
// select random points on the unit circle and decide which point is in and which is not
// then multiply that with 4 and divide by number of experiments to reach pi

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 10000000   // number of experiments
#define P RAND_MAX   // precision

int main() {
    float x = 0, y = 0;
    unsigned int counter = 0;

    srand(time(NULL));

    for(unsigned int i = 0; i < N; i++) {
        x = rand() % P / (float)P;      // create a random integer between 0 and P then normalize it
        y = rand() % P / (float)P;      // create a random integer between 0 and P then normalize it

        if(sqrt(x*x + y*y <= 1)) {      // increase counter if (x,y) point inside of the unit circle 
            counter++;
        }
    }

    float result = 4 * counter / (float)N;
    printf("%f\n", result);

    return 0;
}
