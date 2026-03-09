// Two friends decide to meet. They will both arrive at any time between 12 and 13 o'clock,
// wait fifteen minutes and then leave. If they run into each other during those fifteen
// minutes, they will meet, otherwise they will not meet. What is the probability that
// the two friends will meet?

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N 360000

int main() {
    float a = 0, b = 0;
    unsigned int counter = 0;

    srand(time(NULL));

    for(unsigned int i = 0; i < N; i++) {
        a = rand() % (N+1);
        b = rand() % (N+1);
        if(fabs(a-b) <= 90000) {
            counter++;
        }
    }
    float result = counter / (float)N;
    printf("%f\n", result);

    return 0;
}
