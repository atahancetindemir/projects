#include <stdio.h>

#define TAX 0.15
#define YEAR 365.0
#define LINE_WIDTH 120


// Returns greatest common divisor
unsigned int gcd(unsigned int a, unsigned int b) {
    if(b == 0) return a;
    else return gcd(b, a % b);
}


// Returns least common multiple
unsigned int lcm(unsigned int a, unsigned int b) {
    return (a * b) / gcd(a, b);
}


void print_line(void) {
    for (int i = 0; i < LINE_WIDTH; i++) printf("-");
        printf("\n");
}


void setup(void) {
    printf("%-15s %-15s %-15s %-15s %-15s %-15s %-15s %s\n",
           "Period", "Total", "Interest", "Day",
           "Daily", "Sum interest", "Sum day", "Sum daily");
    print_line();
}


void display(unsigned int index, float total, float net_interest, float sum_interest, unsigned int day, unsigned int sum_day) {

    float daily_income = net_interest / day;
    float sum_daily_income = sum_interest / sum_day;

    printf("%-15u %-15.2f %-15.2f %-15u %-15.2f %-15.2f %-15u %.2f\n",
           index, total, net_interest, day, daily_income, sum_interest, sum_day, sum_daily_income);
}


void calculate_interest(float total, unsigned int day, float ratio, unsigned int frequence) {
    float before_tax_interest, tax, net_interest, sum_interest = 0, sum_day = 0;

    for (unsigned int i = 1; i <= frequence; i++) {
        before_tax_interest = total * ratio * day / YEAR;
        tax = before_tax_interest * TAX;
        net_interest = before_tax_interest - tax;
        total += net_interest;

        sum_day += day;
        sum_interest += net_interest;

        display(i, total, net_interest, sum_interest, day, sum_day);
    }
}


int main(int argc, char* argv[]) {
    unsigned int day, day2, frequence;
    float ratio, ratio2, total;

    if (argc != 6) {
    fprintf(stderr, "Usage: %s {total} {day} {ratio} {day2} {ratio2}\n", argv[0]);
    return 1;
    }

    sscanf(argv[1], "%f", &total);
    sscanf(argv[2], "%u", &day);
    sscanf(argv[3], "%f", &ratio);
    sscanf(argv[4], "%u", &day2);
    sscanf(argv[5], "%f", &ratio2);

    frequence = lcm(day, day2);
    unsigned int corrected_frequence = frequence / day;
    unsigned int corrected_frequence2 = frequence / day2;

    setup();
    calculate_interest(total, day, ratio, corrected_frequence);
    print_line();
    calculate_interest(total, day2, ratio2, corrected_frequence2);

    return 0;
}
