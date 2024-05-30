/* BLAS based implementation of bird flocking simulation */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <cblas.h>
#include "./utils.h"
#include "./params.h"


void initialize_positions(double *x, double *y, int n, double l) {
  for (int i = 0; i < n; i++) {
    x[i] = ((double) rand() / RAND_MAX) * l;
    y[i] = ((double) rand() / RAND_MAX) * l;
  }
}

void initialize_velocities(double *vx, double *vy, double *theta, int n) {
  for (int i = 0; i < n; i++) {
    theta[i] = 2 * M_PI * ((double) rand() / RAND_MAX);
    vx[i] = V0 * cos(theta[i]);
    vy[i] = V0 * sin(theta[i]);
  }
}

void apply_periodic_boundary_conditions(double *x, double *y, int n, double l) {
  for (int i = 0; i < n; i++) {
    x[i] = fmod(x[i], l);
    y[i] = fmod(y[i], l);
    if (x[i] < 0) x[i] += l;
    if (y[i] < 0) y[i] += l;
  }
}

void update_positions_blas(double *x, double *y, double *vx, double *vy, int n, double dt) {
  cblas_daxpy(n, dt, vx, 1, x, 1);
  cblas_daxpy(n, dt, vy, 1, y, 1);
}

void update_positions(double *x, double *y, double *vx, double *vy, int n, double dt) {
  for (int i = 0; i < n; i++) {
    x[i] += vx[i] * dt;
    y[i] += vy[i] * dt;
  }
}

void calculate_mean_theta(double *mean_theta, double *theta, double *x, double *y, int n, double r) {
  for (int b = 0; b < n; b++) {
    double sx = 0.0, sy = 0.0;
    double *cos_theta = (double *)malloc(n * sizeof(double));
    double *sin_theta = (double *)malloc(n * sizeof(double));

    for (int i = 0; i < n; i++) {
      double dx = x[i] - x[b];
      double dy = y[i] - y[b];
      if (dx * dx + dy * dy < r * r) {
        cos_theta[i] = cos(theta[i]);
        sin_theta[i] = sin(theta[i]);
      } else {
        cos_theta[i] = 0.0;
        sin_theta[i] = 0.0;
      }
    }

    sx = cblas_ddot(n, cos_theta, 1, theta, 1);
    sy = cblas_ddot(n, sin_theta, 1, theta, 1);

    free(cos_theta);
    free(sin_theta);

    mean_theta[b] = atan2(sy, sx);
  }
}
void calculate_mean_theta_original(double *mean_theta, double *theta, double *x, double *y, int n, double r) {
  for (int b = 0; b < n; b++) {
    double sx = 0.0, sy = 0.0;
    for (int i = 0; i < n; i++) {
      double dx = x[i] - x[b];
      double dy = y[i] - y[b];
      if (dx * dx + dy * dy < r * r) {
        sx += cos(theta[i]);
        sy += sin(theta[i]);
      }
    }
    mean_theta[b] = atan2(sy, sx);
  }
}

void update_theta(double *theta, double *mean_theta, int n) {
  for (int b = 0; b < n; b++) {
    theta[b] = mean_theta[b] + ETA * (((double) rand() / RAND_MAX) - 0.5);
  }
}

void update_velocities(double *vx, double *vy, double *theta, int n) {
  for (int b = 0; b < n; b++) {
    vx[b] = V0 * cos(theta[b]);
    vy[b] = V0 * sin(theta[b]);
  }
}

int main(int argc, char **argv) {
  int n = parse_n(argc, argv);

  srand(time(NULL));

  double x[n], y[n], vx[n], vy[n], theta[n], mean_theta[n];

  initialize_positions(x, y, n, L);
  initialize_velocities(vx, vy, theta, n);

  double t_start = get_time_ns();
  for (int t = 0; t < NT; t++) {
    update_positions(x, y, vx, vy, n, DT);
    apply_periodic_boundary_conditions(x, y, n, L);
    calculate_mean_theta(mean_theta, theta, x, y, n, R);
    update_theta(theta, mean_theta, n);
    update_velocities(vx, vy, theta, n);
    if (PRINT) print_flock_positions(t, x, y, vx, vy, n);
  }
  double t_end = get_time_ns();
  print_time(time_to_unit(t_end - t_start, "ns", TIME_UNIT), TIME_UNIT);

  printf("Simulation complete.\n");
  return 0;
}

