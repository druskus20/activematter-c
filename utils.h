#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "params.h"

/**
 * @brief Parse the number of birds from command line arguments.
 * 
 * @param argc The number of command line arguments.
 * @param argv The array of command line arguments.
 * @return The number of birds specified in the command line arguments or the default value.
 */
size_t parse_n(int argc, char **argv) {
    size_t n = N_DEFAULT;
    if (argc >= 2) {
        char *endptr;
        long int parsed = strtol(argv[1], &endptr, 10);
        if (*endptr == '\0') { // Check if conversion was successful
            n = (size_t)parsed;
        }
    }
    return n;
}

/**
 * @brief Get the current time in nanoseconds.
 * 
 * @return The current time in nanoseconds.
 */
double get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1e9 + ts.tv_nsec;
}

/**
 * @brief Convert time from source unit to target unit.
 * 
 * @param time The time to convert.
 * @param source_unit The unit of the input time ("ns", "us", "ms", "s").
 * @param target_unit The desired unit of the output time ("ns", "us", "ms", "s").
 * @return The converted time in the target unit.
 */
double time_to_unit(double time, char *source_unit, char *target_unit) {
    double conversion_factor = 1.0; 
    if (strcmp(source_unit, "ns") == 0) {
        conversion_factor /= 1e9; 
    } else if (strcmp(source_unit, "us") == 0) {
        conversion_factor /= 1e6; 
    } else if (strcmp(source_unit, "ms") == 0) {
        conversion_factor /= 1e3; 
    }
    
    double time_in_seconds = time * conversion_factor;
    
    if (strcmp(target_unit, "ns") == 0) {
        return time_in_seconds * 1e9; 
    } else if (strcmp(target_unit, "us") == 0) {
        return time_in_seconds * 1e6; 
    } else if (strcmp(target_unit, "ms") == 0) {
        return time_in_seconds * 1e3; 
    } else if (strcmp(target_unit, "s") == 0) {
        return time_in_seconds; 
    } else {
        printf("Unsupported target unit: %s\n", target_unit);
        return -1.0; 
    }
}

/**
 * @brief Print the simulation time.
 * 
 * @param t The time to print.
 * @param unit The unit of the time.
 */
void print_time(double t, char *unit) {
  printf("Time: %f %s\n", t, unit);
}

/**
 * @brief Print the positions and velocities of the flock.
 * 
 * @param step The current time step.
 * @param x The array of x positions.
 * @param y The array of y positions.
 * @param vx The array of x velocities.
 * @param vy The array of y velocities.
 * @param n The number of birds.
 */
void print_flock_positions(int step, double *x, double *y, double *vx, double *vy, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d %d %f %f %f %f\n", step, i, x[i], y[i], vx[i], vy[i]);
    }
}

#endif

