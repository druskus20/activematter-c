#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./utils.h"
#include "./params.h"
#include <mpi.h>

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

void update_positions(double *x, double *y, double *vx, double *vy, int n, double dt) {
    for (int i = 0; i < n; i++) {
        x[i] += vx[i] * dt;
        y[i] += vy[i] * dt;
    }
}

void calculate_mean_theta(double *mean_theta, double *theta, double *x, double *y, int n, double r, int start, int end) {
    for (int b = start; b < end; b++) {
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

void update_theta(double *theta, double *mean_theta, int n, int start, int end) {
    for (int b = start; b < end; b++) {
        theta[b] = mean_theta[b] + ETA * (((double) rand() / RAND_MAX) - 0.5);
    }
}

void update_velocities(double *vx, double *vy, double *theta, int n, int start, int end) {
    for (int b = start; b < end; b++) {
        vx[b] = V0 * cos(theta[b]);
        vy[b] = V0 * sin(theta[b]);
    }
}

int main(int argc, char **argv) {
    int n = parse_n(argc, argv);
    srand(1);

    double x[n], y[n], vx[n], vy[n], theta[n], mean_theta[n];

    double t_start = get_time_ns();

    // Initialize MPI
    int rank, num_ranks;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

    int birds_per_proc = n / num_ranks;
    int start = rank * birds_per_proc;
    int end = (rank == num_ranks - 1) ? n : start + birds_per_proc;

    initialize_velocities(vx, vy, theta, n);
    initialize_positions(x, y, n, L);

    // Allocate separate send and receive buffers for MPI
    double local_mean_theta[birds_per_proc];
    double local_theta[birds_per_proc];
    double local_vx[birds_per_proc];
    double local_vy[birds_per_proc];

    for (int t = 0; t < NT; t++) {
        update_positions(x, y, vx, vy, n, DT);
        apply_periodic_boundary_conditions(x, y, n, L);

        // calculate mean_theta with MPI
        calculate_mean_theta(local_mean_theta, theta, x, y, n, R, start, end);
        MPI_Allgather(local_mean_theta, birds_per_proc, MPI_DOUBLE, mean_theta, birds_per_proc, MPI_DOUBLE, MPI_COMM_WORLD);

        // update theta and velocities with MPI
        update_theta(local_theta, mean_theta, n, start, end);
        update_velocities(local_vx, local_vy, local_theta, n, start, end);

        MPI_Allgather(local_theta, birds_per_proc, MPI_DOUBLE, theta, birds_per_proc, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Allgather(local_vx, birds_per_proc, MPI_DOUBLE, vx, birds_per_proc, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Allgather(local_vy, birds_per_proc, MPI_DOUBLE, vy, birds_per_proc, MPI_DOUBLE, MPI_COMM_WORLD);

        if (PRINT) print_flock_positions(t, x, y, vx, vy, n);
    }

    double t_end = get_time_ns();
    print_time(time_to_unit(t_end - t_start, "ns", TIME_UNIT), TIME_UNIT);

    MPI_Finalize();

    return 0;
}

