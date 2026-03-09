// https://en.wikipedia.org/wiki/Numerical_differentiation

#include <stdio.h>
#include <math.h>

#define ITERATION_NUM 10
#define LINE_WIDTH    160

// If you change f you should change f_exact and f_exact_second manually as well.

// y = 4x^3 + 2x^2 - 8
double f(double x) {
    return 4*x*x*x + 2*x*x - 8;
}


// dy/dx(4x^3 + 2x^2 - 8)
double f_exact(double x) {
    return 12*x*x + 4*x;
}


// Unused
// d^2y/dx^2(4x^3 + 2x^2 - 8)
double f_exact_second(double x) {
    return 24*x + 4;
}


double forward_difference(double x, double h) {
    return (f(x + h) - f(x)) / h;
}


double backward_difference(double x, double h) {
    return (f(x) - f(x - h)) / h;
}


double central_difference(double x, double h) {
    return (f(x + h) - f(x - h)) / (2 * h);
}


// Unused
double three_point_forward_difference(double x, double h) {
    return (-3 * f(x) + 4 * f(x + h) - f(x + 2 * h)) / (2 * h);
}


// Unused
double three_point_backward_difference(double x, double h) {
    return (f(x - 2 * h) - 4 * f(x - h) + 3 * f(x)) / (2 * h);
}


// Unused
double three_point_central_difference(double x, double h) {
    return (f(x + h) - f(x - h)) / (2 * h);
}


// Unused
double three_point_second_order_forward_difference(double x, double h) {
    return (f(x) - 2 * f(x + h) + f(x + 2 * h)) / (h * h);
}


// Unused
double three_point_second_order_backward_difference(double x, double h) {
    return (f(x - 2 * h) - 2 * f(x - h) + f(x)) / (h * h);
}


// Unused
double three_point_second_order_central_difference(double x, double h) {
    return (f(x - h) - 2 * f(x) + f(x + h)) / (h * h);
}


double calculate_error(double measured_value, double real_value) {
    return fabs(measured_value - real_value) / real_value * 100;
}


void setup(void) {
    printf("%-15s %-15s %-20s %-20s %-20s %-20s %-17s %-17s %s\n",
           "x", "h", "exact", "forward-diff", "backward-diff",
           "central-diff", "forward-rel", "backward-rel", "central-rel");

    for (int i = 0; i < LINE_WIDTH; i++) printf("-");
    printf("\n");
}


void display(double x, double h) {

    double real_value = f_exact(x);
    double forward_value = forward_difference(x, h);
    double backward_value = backward_difference(x, h);
    double central_value = central_difference(x, h);

    double forward_error = calculate_error(forward_value, real_value);
    double backward_error = calculate_error(backward_value, real_value);
    double central_error = calculate_error(central_value, real_value);

    printf("%-15.4lf %-15lf %-20lf %-20lf %-20lf %-20lf %-17lf %-17lf %lf\n",
           x, h, real_value,
           forward_value, backward_value, central_value,
           forward_error, backward_error, central_error);
}


int main() {
    double x = 28; // random val
    double h = 1;

    setup();

    for(unsigned int i = 0; i < ITERATION_NUM; ++i) {
        display(x, h);
        h /= 2; // Increase precision for the next iteration.
    }

    return 0;
}