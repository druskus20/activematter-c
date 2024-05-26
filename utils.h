#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <time.h>

double get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1e9 + ts.tv_nsec;
}

double ns_to_s(double ns) {
    return ns / 1e9;
}

double s_to_ns(double s) {
    return s * 1e9;
}

void print_time(double t, char *unit) {
  printf("Time: %f %s\n", t, unit);
}

void print_data(double *data, int size) {
  for (int i = 0; i < size; i++) {
    printf("%f\n", data[i]);
  }
}

#endif

