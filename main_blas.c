/* BLAS based implementation of bird flocking simulation */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <cblas.h>

#define V0 1.0
#define ETA 0.5
#define L 10.0
#define R 1.0
#define DT 0.2
#define NT 200
#define N 500

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

int main() {
    srand(time(NULL));
    
    double x[N], y[N], vx[N], vy[N], theta[N], mean_theta[N];
    
    initialize_positions(x, y, N, L);
    initialize_velocities(vx, vy, theta, N);
    
    for (int t = 0; t < NT; t++) {
        update_positions(x, y, vx, vy, N, DT);
        apply_periodic_boundary_conditions(x, y, N, L);
        calculate_mean_theta(mean_theta, theta, x, y, N, R);
        update_theta(theta, mean_theta, N);
        update_velocities(vx, vy, theta, N);
    }
    
    printf("Simulation complete.\n");
    return 0;
}
