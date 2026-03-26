// https://en.wikipedia.org/wiki/Numerical_integration

#include <stdio.h>
#include <math.h>

#define REAL        3.14159265358979323846  // analytical solution for 4.0 / (1.0 + x * x)
#define MAX_COLUMN  4294967296              // maximum column number
#define START_VALUE 0
#define END_VALUE   1
#define LINE_WIDTH  120

double f(double x) {
    return 4.0 / (1.0 + x * x);
}

double simpsons_13(double (*f)(double), unsigned long n, double a, double b) {
    if (n % 2 != 0) {
        n++;
    }
    
    double sum = f(a) + f(b);
    double h = (b - a) / n;

    for (unsigned long i = 1; i < n; i++) {
        double x = a + i * h;

        if (i % 2 != 0) {
            sum += 4.0 * f(x);
        } else {
            sum += 2.0 * f(x);
        }
        
    }
    return (h / 3.0) * sum;
}

/**
 * @brief Calculate the integral of f from a to b using the trapezoidal rule with n rectangles.
 * @param f function to integrate
 * @param n rectangle count
 * @param a start value
 * @param b end value
 * @return the integral of f from a to b
 */
double trapezoidal(double (*f)(double), unsigned long n, double a, double b) {
    double sum = (f(a) + f(b)) / 2.0;               // initialize with avg of start and end val
    double h = (b - a) / n;                         // interval

    for (unsigned long i = 1; i < n; i++) {         // 1 to n - 1 (or a+h to b-h)
        double x = a + i * h;                       // f(a+h) + f(a+h+h) + ... + f(b-h)    
        sum += f(x);                                       
    }
    return h * sum;
}

double rectangular_left_endpoint(double (*f)(double), unsigned long n, double a, double b) {
    double sum = 0.0;
    double h = (b - a) / n;

    for (unsigned long i = 0; i < n; i++) {         // 0 to n - 1 (or a to b-h)                  
        double x = a + (i + 0.0) * h;               // f(a) + f(a+h) + ... + f(b-h)
        sum += f(x);                
    }
    return h * sum;
}

double riemann_sum(double (*f)(double), unsigned long n, double a, double b) {
    double sum = 0.0;
    double h = (b - a) / n;

    for (unsigned long i = 0; i < n; i++) {           
        double x = a + (i + 0.5) * h;
        sum += f(x);
    }
    return h * sum;
}

double rectangular_right_endpoint(double (*f)(double), unsigned long n, double a, double b) {
    double sum = 0.0;
    double h = (b - a) / n;
    
    for (unsigned long i = 0; i < n; i++) {         // 1 to n (or a+h to b)                  
        double x = a + (i + 1.0) * h;               // f(a+h) + f(a+h+h) + ... + f(b)
        sum += f(x);                
    }
    return h * sum;
}

double calculate_error(double measured_value, double real_value) {
    return fabs(measured_value - real_value) / real_value * 100;
}

void display(unsigned long column_number) {

    double rect_right_value = rectangular_right_endpoint(f, column_number, START_VALUE, END_VALUE);
    double rect_left_value = rectangular_left_endpoint(f, column_number, START_VALUE, END_VALUE);
    double trapezoidal_value = trapezoidal(f, column_number, START_VALUE, END_VALUE);
    double riemann_value = riemann_sum(f, column_number, START_VALUE, END_VALUE);
    double simpsons_value = simpsons_13(f, column_number, START_VALUE, END_VALUE);
    
    double rect_right_error = calculate_error(rect_right_value, REAL);
    double rect_left_error = calculate_error(rect_left_value, REAL);
    double trapezoidal_error = calculate_error(trapezoidal_value, REAL);
    double riemann_error = calculate_error(riemann_value, REAL);
    double simpsons_error = calculate_error(simpsons_value, REAL);
    
    printf("%-20lu %-20.15lf %-20.15lf %-20.15lf %-20.15lf %.15lf\n",
           column_number, rect_right_error, rect_left_error, trapezoidal_error, riemann_error, simpsons_error);
}

void setup(void) {
    printf("%-20s %-20s %-20s %-20s %-20s %s\n",
           "Columns", "Right-Err", "Left-Err", "Trapezoidal-Err", "Riemann-Err", "Simpson-Err");

    for (int i = 0; i < LINE_WIDTH; i++) printf("-");
    printf("\n");
}

int main() {
    unsigned long column_number = 0x2;

    setup();

    while(MAX_COLUMN >= column_number) {

        display(column_number);

        column_number <<= 1;
    }

    return 0;
}

/*
gcc -O3 -march=native numerical_integration.c -o numerical_integration -lm
./numerical_integration

Columns              Right-Err            Left-Err             Trapezoidal-Err      Riemann-Err          Simpson-Err
------------------------------------------------------------------------------------------------------------------------
2                    17.239429592214421   14.591559026164649   1.323935283024885    0.660820477885800    0.262902329078921
4                    8.289304557164302    7.626189752025231    0.331557402569536    0.165771485306942    0.000764775751087
8                    4.061766535928688    3.895980618666080    0.082892958631304    0.041446366396610    0.000004810651894
16                   2.010160084766039    1.968713492531345    0.020723296117347    0.010361646294116    0.000000075279372
32                   0.999899219235947    0.989537569412744    0.005180824911601    0.002590412428271    0.000000001176387
64                   0.498654403403902    0.496063990920430    0.001295206241729    0.000647603120377    0.000000000018401
128                  0.249003400141720    0.248355797020467    0.000323801560634    0.000161900780345    0.000000000000307
256                  0.124420749680744    0.124258848900342    0.000080950390201    0.000040475195030    0.000000000000013
512                  0.062190137242624    0.062149662047919    0.000020237597352    0.000010118798733    0.000000000000013
1024                 0.031090009222059    0.031079890423199    0.000005059399437    0.000002529699683    0.000000000000043
2048                 0.015543739761287    0.015541210061349    0.000001264849969    0.000000632424885    0.000000000000062
4096                 0.007771553668186    0.007770921243131    0.000000316212528    0.000000158106165    0.000000000000006
8192                 0.003885697780742    0.003885539674903    0.000000079052927    0.000000039526577    0.000000000000137
16384                0.001942829127500    0.001942789600330    0.000000019763585    0.000000009881333    0.000000000000221
32768                0.000971409622914    0.000971399741001    0.000000004940956    0.000000002470471    0.000000000000345
65536                0.000485703576645    0.000485701105312    0.000000001235667    0.000000000616999    0.000000000000100
131072               0.000242851478409    0.000242850862569    0.000000000307906    0.000000000154066    0.000000000001672
262144               0.000121425661437    0.000121425509053    0.000000000076192    0.000000000040754    0.000000000002426
524288               0.000060712812469    0.000060712772762    0.000000000019847    0.000000000010079    0.000000000001231
1048576              0.000030356400354    0.000030356392268    0.000000000004043    0.000000000000127    0.000000000000748
2097152              0.000015178200884    0.000015178195427    0.000000000002728    0.000000000000410    0.000000000001268
4194304              0.000007589099078    0.000007589099078    0.000000000000000    0.000000000010927    0.000000000003436
8388608              0.000003794554359    0.000003794544719    0.000000000004820    0.000000000013627    0.000000000007854
16777216             0.000001897256647    0.000001897292892    0.000000000018150    0.000000000005428    0.000000000020708
33554432             0.000000948644241    0.000000948630529    0.000000000006856    0.000000000020907    0.000000000023980
67108864             0.000000474342186    0.000000474295199    0.000000000023494    0.000000000038873    0.000000000008985
134217728            0.000000237148377    0.000000237170316    0.000000000010969    0.000000000000396    0.000000000009806
268435456            0.000000118577100    0.000000118582232    0.000000000002559    0.000000000015931    0.000000000006685
536870912            0.000000059285002    0.000000059294671    0.000000000004834    0.000000000011634    0.000000000023931
1073741824           0.000000029634373    0.000000029655464    0.000000000010545    0.000000000006008    0.000000000012294
2147483648           0.000000014824141    0.000000014820763    0.000000000001696    0.000000000000608    0.000000000001155
4294967296           0.000000007412064    0.000000007410396    0.000000000000834    0.000000000000834    0.000000000006659
*/