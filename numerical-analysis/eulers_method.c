// https://en.wikipedia.org/wiki/Euler_method

#include <stdio.h>

#define h         0.1
#define Y_START   1 // y(0) = 1
#define X_START   0
#define X_STOP    1 // y(1) = ?
#define ITERATION (X_STOP - X_START) / h

// ODE: dy/dx = (x - y)/2
double f(double x, double y) {
    return (x - y) / 2;
}

double euler(double x, double y) {
    return y + f(x,y) * h;;
}

int main() {
    double x = X_START, y = Y_START;

    for(unsigned int i = 0; i <= ITERATION; ++i) {
        printf("y(%.1lf) = %lf\n", x, y);
        y = euler(x, y);
        x += h;
    }

    return 0;
}
