// https://en.wikipedia.org/wiki/Numerical_integration

#include <stdio.h>
#include <math.h>

#define REAL        0.78539816339    // analytical solution for sqrt(1-x*x)
#define MAX_COLUMN  (0xFFFFFF + 0x1) // maximum column number
#define START_VALUE 0
#define END_VALUE   1
#define LINE_WIDTH  120


double f(double x) {
    return sqrt(1-x*x); // quarter circle
}


double riemann_sum(void) {
    return 0.0;
}


double simpsons_rule(void) {
    return 0.0;
}

/**
 * @brief trapezoidal rule
 * @param a start value
 * @param b end value
 * @param n rectangle count
 * @return numerical solution of the integral
 */
double trapezoidal(unsigned int n, double a, double b) {
    double sum = (f(a) + f(b)) / 2.0;   // initialize with avg of start and end val
    double h = (b - a) / n;             // interval

    for (double i = a+h; i < b; i+=h) { // 1 to n - 1 (or a+h to b-h)
        sum += f(i);                    // f(a+h) + f(a+h+h) + ... + f(b-h)
    }
    return h * sum;
}


double rectangular_left_endpoint(unsigned int n, double a, double b) {
    double sum = 0;
    double h = (b - a) / n;

    for (double i = a; i < b; i+=h) { // 0 to n - 1 (or a to b-h)
        sum += f(i);                  // f(a) + f(a+h) + ... + f(b-h)
    }
    return h * sum;
}


double rectangular_right_endpoint(unsigned int n, double a, double b) {
    double sum = 0;
    double h = (b - a) / n;
    
    for (double i = a+h; i <= b; i+=h) { // 1 to n (or a+h to b)
        sum += f(i);                     // f(a+h) + f(a+h+h) + ... + f(b)
    }
    return h * sum;
}


double calculate_error(double measured_value, double real_value) {
    return fabs(measured_value - real_value) / real_value * 100;
}


void display(unsigned int column_number) {

    double rect_left_value = rectangular_left_endpoint(column_number, START_VALUE, END_VALUE);
    double rect_right_value = rectangular_right_endpoint(column_number, START_VALUE, END_VALUE);
    double trapezoidal_value = trapezoidal(column_number, START_VALUE, END_VALUE);

    double rect_left_error = calculate_error(rect_left_value, REAL);
    double rect_right_error = calculate_error(rect_right_value, REAL);
    double trapezoidal_error = calculate_error(trapezoidal_value, REAL);

    printf("%-15u %-15lf %-15lf %-15lf %-15lf %-15lf %-15lf %lf\n",
           column_number, REAL,
           rect_left_value, rect_right_value, trapezoidal_value,
           rect_left_error, rect_right_error, trapezoidal_error);
}


void setup(void) {
    printf("%-15s %-15s %-15s %-15s %-15s %-15s %-15s %s\n",
           "Columns", "Exact Sol", "Left Rect", "Right Rect","Trapezoidal",
           "Left-Rel", "Right-Rel", "Trap-Rel");

    for (int i = 0; i < LINE_WIDTH; i++) printf("-");
    printf("\n");
}


int main() {
    unsigned int column_number = 0x2;

    setup();

    while(MAX_COLUMN >= column_number) {

        display(column_number);

        column_number <<= 1;
    }

    return 0;
}
