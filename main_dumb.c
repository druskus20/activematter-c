/* Naive implementation of bird flocking simulation */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./utils.h"
#include "./params.h"

/**
 * @brief Initializes the positions of birds randomly within a square of side length l.
 * 
 * @param x Pointer to the array of x coordinates.
 * @param y Pointer to the array of y coordinates.
 * @param n Number of birds.
 * @param l Side length of the square.
 */
void initialize_positions(double *x, double *y, int n, double l) {
  for (int i = 0; i < n; i++) {
    x[i] = ((double) rand() / RAND_MAX) * l;
    y[i] = ((double) rand() / RAND_MAX) * l;
  }
}

/**
 * @brief Initializes the velocities of birds with random directions and a fixed speed.
 * 
 * @param vx Pointer to the array of x components of velocity.
 * @param vy Pointer to the array of y components of velocity.
 * @param theta Pointer to the array of angles representing the direction of velocity.
 * @param n Number of birds.
 */
void initialize_velocities(double *vx, double *vy, double *theta, int n) {
  for (int i = 0; i < n; i++) {
    theta[i] = 2 * M_PI * ((double) rand() / RAND_MAX);
    vx[i] = V0 * cos(theta[i]);
    vy[i] = V0 * sin(theta[i]);
  }
}

/**
 * @brief Applies periodic boundary conditions to ensure birds stay within the square.
 * 
 * @param x Pointer to the array of x coordinates.
 * @param y Pointer to the array of y coordinates.
 * @param n Number of birds.
 * @param l Side length of the square.
 */
void apply_periodic_boundary_conditions(double *x, double *y, int n, double l) {
  for (int i = 0; i < n; i++) {
    x[i] = fmod(x[i], l);
    y[i] = fmod(y[i], l);
    if (x[i] < 0) x[i] += l;
    if (y[i] < 0) y[i] += l;
  }
}

/**
 * @brief Updates the positions of birds based on their velocities and a time step.
 * 
 * @param x Pointer to the array of x coordinates.
 * @param y Pointer to the array of y coordinates.
 * @param vx Pointer to the array of x components of velocity.
 * @param vy Pointer to the array of y components of velocity.
 * @param n Number of birds.
 * @param dt Time step for the update.
 */
void update_positions(double *x, double *y, double *vx, double *vy, int n, double dt) {
  for (int i = 0; i < n; i++) {
    x[i] += vx[i] * dt;
    y[i] += vy[i] * dt;
  }
}

/**
 * @brief Calculates the mean direction (theta) of nearby birds for each bird.
 * 
 * @param mean_theta Pointer to the array of mean directions.
 * @param theta Pointer to the array of current directions.
 * @param x Pointer to the array of x coordinates.
 * @param y Pointer to the array of y coordinates.
 * @param n Number of birds.
 * @param r Radius within which to consider neighboring birds.
 */
void calculate_mean_theta(double *mean_theta, double *theta, double *x, double *y, int n, double r) {
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

/**
 * @brief Updates the directions (theta) of birds based on the mean directions and some noise.
 * 
 * @param theta Pointer to the array of current directions.
 * @param mean_theta Pointer to the array of mean directions.
 * @param n Number of birds.
 */
void update_theta(double *theta, double *mean_theta, int n) {
  for (int b = 0; b < n; b++) {
    theta[b] = mean_theta[b] + ETA * (((double) rand() / RAND_MAX) - 0.5);
  }
}

/**
 * @brief Updates the velocities of birds based on their updated directions (theta).
 * 
 * @param vx Pointer to the array of x components of velocity.
 * @param vy Pointer to the array of y components of velocity.
 * @param theta Pointer to the array of current directions.
 * @param n Number of birds.
 */
void update_velocities(double *vx, double *vy, double *theta, int n) {
  for (int b = 0; b < n; b++) {
    vx[b] = V0 * cos(theta[b]);
    vy[b] = V0 * sin(theta[b]);
  }
}

/**
 * @brief Main function to simulate bird flocking.
 * 
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return int Exit status.
 */
int main(int argc, char **argv) {
  // Parse the number of birds from command line arguments
  int n = parse_n(argc, argv);
  srand(1);

  // Arrays for positions, velocities, and angles
  double x[n], y[n], vx[n], vy[n], theta[n], mean_theta[n];

  // Record the start time
  double t_start = get_time_ns();

  // Initialize velocities and positions
  initialize_velocities(vx, vy, theta, n);
  initialize_positions(x, y, n, L);

  // Main simulation loop
  for (int t = 0; t < NT; t++) {
    update_positions(x, y, vx, vy, n, DT);
    apply_periodic_boundary_conditions(x, y, n, L);
    calculate_mean_theta(mean_theta, theta, x, y, n, R);
    update_theta(theta, mean_theta, n);
    update_velocities(vx, vy, theta, n);
    if (PRINT) print_flock_positions(t, x, y, vx, vy, n);
  }

  // Record the end time and print the elapsed time
  double t_end = get_time_ns();
  print_time(time_to_unit(t_end - t_start, "ns", TIME_UNIT), TIME_UNIT);

  return 0;
}

