#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

double get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1e9 + ts.tv_nsec;
}

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

void print_time(double t, char *unit) {
  printf("Time: %f %s\n", t, unit);
}

void print_flock_positions(int step, double *x, double *y, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d %d %f %f\n", step, i, x[i], y[i]);
    }
}

#endif

