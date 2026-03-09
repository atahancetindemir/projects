// https://www.youtube.com/watch?v=WZg2fp7TOcE

#include <stdio.h>

// direct approach
double calculate_direct(double velocity_a, double velocity_b, double velocity_fly, double total_x) {
    double time = total_x / (velocity_a + velocity_b);
    return time * velocity_fly;
}

// numerical approach
double calculate_numerical(double velocity_a, double velocity_b, double velocity_fly, double total_x, unsigned int step_count) {
    double t_impact = 0, x_current_a = 0, x_current_b = 0, x_sum_fly = 0;
    
    for (unsigned int i = 0; i < step_count; i++) {
        
        if (i % 2 == 0) {                                       // this is needed for different car speeds
            t_impact = total_x / (velocity_b + velocity_fly);   // movement towards b
        }
        else {              
            t_impact = total_x / (velocity_a + velocity_fly);   // movement towards a
        }

        x_current_b = t_impact * velocity_b;
        x_current_a = t_impact * velocity_a;
        x_sum_fly += t_impact * velocity_fly;
        total_x -= x_current_a + x_current_b;

    }
    return x_sum_fly;
}

int main(int argc, char* argv[]) {
    double velocity_a = 0, velocity_b = 0, velocity_f = 0, total_x = 0;
    unsigned int step_count = 0;

    if (argc == 6) {
        sscanf(argv[1], "%lf", &velocity_a);
        sscanf(argv[2], "%lf", &velocity_b);
        sscanf(argv[3], "%lf", &velocity_f);
        sscanf(argv[4], "%lf", &total_x);
        sscanf(argv[5], "%u", &step_count);

        printf("direct approach: %.8lf\n", calculate_direct(velocity_a, velocity_b, velocity_f, total_x));
        printf("numerical approach: %.8lf\n", calculate_numerical(velocity_a, velocity_b, velocity_f, total_x, step_count));
        printf("step count: %u\n", step_count);
    }   

    return 0;

}
