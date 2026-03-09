// https://en.wikipedia.org/wiki/Heun%27s_method

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

double heun(double x, double y) {
    return y + (f(x,y) + f(x+h, y + f(x,y) * h)) / 2.0 * h;
}

int main() {
    double x = X_START, y = Y_START;

    for(unsigned int i = 0; i <= ITERATION; ++i) {
        printf("y(%.1lf) = %lf\n", x, y);
        y = heun(x, y);
        x += h;
    }

    return 0;
}
