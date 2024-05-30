# Compiler
CC = gcc

PROFILE_CFLAGS =  
#PROFILE_CFLAGS =  -fprofile-arcs -ftest-coverage 
# Compiler flags
#CFLAGS = $(PROFILE_CFLAGS) 
#CFLAGS = $(PROFILE_CFLAGS) -O3 # Optimize for speed
CFLAGS = $(PROFILE_CFLAGS) -O3 -march=native -funroll-loops -ffast-math # Hard optimize for speed

# Libraries
LIBS = -lm -lblas -fopenmp 

# Target executables
TARGETS = blas dumb omp mpi
PROFILE_TARGETS = dumb_profile

# Source files
BLAS_SRCS = main_blas.c 
DUMB_SRCS = main_dumb.c 
OMP_SRCS = main_omp.c 
MPI_SRCS = main_mpi.c 
EXTRA = 

# Object files
BLAS_OBJS = $(BLAS_SRCS:.c=.o) 
DUMB_OBJS = $(DUMB_SRCS:.c=.o)
OMP_OBJS = $(OMP_SRCS:.c=.o)
MPI_OBJS = $(MPI_SRCS:.c=.o)

# Default target
all: $(TARGETS)

# BLAS target
blas: $(BLAS_OBJS) 
	$(CC) $(CFLAGS) -o blas $(EXTRA) $(BLAS_OBJS) $(LIBS)

# Dumb target
dumb: $(DUMB_OBJS)
	$(CC) $(CFLAGS) -o dumb $(EXTRA) $(DUMB_OBJS) $(LIBS)

# OpenMP target
omp: $(OMP_OBJS)
	$(CC) $(CFLAGS) -o omp $(EXTRA) $(OMP_OBJS) $(LIBS)

mpi: $(MPI_OBJS)
	mpicc $(CFLAGS) -o mpi $(EXTRA) $(MPI_OBJS) $(LIBS)

main_mpi.o: main_mpi.c 
	mpicc $(CFLAGS) -c $< -o $@ $(LIBS)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)


# Clean up build files
clean:
	rm -f $(BLAS_OBJS) $(DUMB_OBJS) $(OMP_OBJS) $(TARGETS) 

clean_profile: 
	rm -rf *.gcda *.gcno *.gcov

# PHONY targets to avoid conflicts with files of the same name
.PHONY: all clean_profile clean

