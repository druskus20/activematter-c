#ifndef PARAMS_H
#define PARAMS_H

#define PRINT 0 // Set to 1 if you want to print flock positions during the simulation
#define TIME_UNIT "s" // Units for timing: "s" for seconds, "ms" for milliseconds, "us" for microseconds, "ns" for nanoseconds

// Simulation parameters
#define V0 1.0 // Speed of each bird
#define ETA 0.5 // Noise parameter affecting the change in direction
#define L 100.0 // Size of the simulation area (length of the side of the square)
#define R 1.0 // Radius within which birds consider their neighbors
#define DT 0.2 // Time step for each update
#define NT 1000 // Number of time steps to simulate
#define N_DEFAULT 5000 // Default number of birds

#endif

