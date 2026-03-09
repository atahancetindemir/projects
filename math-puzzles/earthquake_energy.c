#include <stdio.h>
#include <math.h>

#define TNT 4184000000
#define HIROSHIMA (TNT * 15000)
#define NAGASAKI (TNT * 21000)

double calculate_energy_release(double magnitude) {
    return pow(10, 1.5 * magnitude + 4.8);
}

double calculate_energy_difference(double mag1, double mag2) {
    return pow(10, 1.5 * fabs(mag1 - mag2));
}

double calculate_magnitude_difference(double mag1, double mag2) {
    return pow(10, fabs(mag1-mag2));
}

double calculate_equivalent(double energy, char type) {
    if (type == 't') return energy / TNT;
    else if (type == 'h') return energy / HIROSHIMA;
    else if (type == 'n') return energy / NAGASAKI;
    else return -1;
}

int main() {
    double mag1, mag2, mag1_energy, mag2_energy, energy_diff, mag_diff = 0;
    
    printf("mag1: ");
    scanf("%lf",&mag1);
    printf("mag2: ");
    scanf("%lf",&mag2);
    
    mag1_energy = calculate_energy_release(mag1);
    mag2_energy = calculate_energy_release(mag2);
    energy_diff = calculate_energy_difference(mag1, mag2);
    mag_diff = calculate_magnitude_difference(mag1, mag2);

    printf("mag1's energy %e joules\n", mag1_energy);
    printf("mag2's energy %e joules\n", mag2_energy);

    printf("mag1's tnt equivalent %lf t\n", calculate_equivalent(mag1_energy, 't'));
    printf("mag2's tnt equivalent %lf t\n", calculate_equivalent(mag2_energy, 't'));

    printf("mag1's hiroshima equivalent %lf\n", calculate_equivalent(mag1_energy, 'h'));
    printf("mag2's hiroshima equivalent %lf\n", calculate_equivalent(mag2_energy, 'h'));

    printf("mag1's nagasaki equivalent %lf\n", calculate_equivalent(mag1_energy, 'n'));
    printf("mag2's nagasaki equivalent %lf\n", calculate_equivalent(mag2_energy, 'n'));

    printf("energy difference: %lf\n", energy_diff);
    printf("magnitude difference: %lf\n", mag_diff);

    return 0;
}
