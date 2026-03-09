#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define PI 3.14159265359
#define E 2.71828182846

// https://en.wikipedia.org/wiki/Stirling%27s_approximation

double stirling(double n) {
    return sqrt(2 * PI * n) * pow((n / E), n);
}

int main(int argc, char* argv[]) {
    double n;

    if (argc == 2) {
        sscanf(argv[1], "%lf", &n);

        printf("%lf\n",stirling(n));
    }

    return 0;
}