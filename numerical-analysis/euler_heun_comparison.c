#include <stdio.h>
#include <math.h>


#define h          0.1
#define Y_START    1.0 // y(0) = 1
#define X_START    0.0
#define X_STOP     1.0 // y(1) = ?
#define ITERATION  fabs(X_STOP - X_START) / h
#define LINE_WIDTH 115

// Exact solution: y = x - 2 + C / exp(x/2)
// Solve IVP x = 0, y = 1 therefore C = 3
double f_exact(double x) {
    return x - 2.0 + 3.0 / exp(x / 2.0);
}


// ODE: dy/dx = (x - y)/2
double f(double x, double y) {
    return (x - y) / 2.0;
}


double euler(double x, double y) {
    return y + f(x,y) * h;
}


double heun(double x, double y) {
    double predictor = y + f(x, y) * h; // avoid function calls
    return y + (f(x,y) + f(x+h, predictor)) / 2.0 * h;
}


double calculate_absolute_error(double measured_value, double real_value) {
    return fabs(measured_value - real_value);
}


double calculate_relative_error(double measured_value, double real_value) {
    return fabs(measured_value - real_value) / real_value * 100;
}


void setup(void) {
    printf("%-10s %-15s %-15s %-15s %-15s %-15s %-15s %s\n",
           "x", "y (Exact)", "y (Euler)", "y (Heun)",
           "Euler-Abs", "Heun-Abs", "Euler-Rel", "Heun-Rel");

    for (int i = 0; i < LINE_WIDTH; i++) printf("-");
    printf("\n");
}


void display(double* x, double* y_euler, double* y_heun) {

    double y_exact = f_exact(*x);
    double euler_abs_error = calculate_absolute_error(*y_euler, y_exact);
    double heun_abs_error = calculate_absolute_error(*y_heun, y_exact);
    double euler_rel_error = calculate_relative_error(*y_euler, y_exact);
    double heun_rel_error = calculate_relative_error(*y_heun, y_exact);

    printf("%-10.1lf %-15lf %-15lf %-15lf %-15lf %-15lf %-15lf %lf\n",
           *x, y_exact, *y_euler, *y_heun,
           euler_abs_error, heun_abs_error, euler_rel_error, heun_rel_error);

    *y_euler = euler(*x, *y_euler);
    *y_heun = heun(*x, *y_heun);
    *x += h;
    
}


int main() {
    double x = X_START;
    double y_euler = Y_START;
    double y_heun = Y_START;

    setup();

    for(unsigned int i = 0; i <= ITERATION; ++i) {
        display(&x, &y_euler, &y_heun);
    }

    return 0;
}
