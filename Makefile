# Compiler
CC = gcc

PROFILE_CFLAGS =  
#PROFILE_CFLAGS =  -fprofile-arcs -ftest-coverage 
# Compiler flags
CFLAGS = $(PROFILE_CFLAGS) 
#CFLAGS = $(PROFILE_CFLAGS) -O3 # Optimize for speed
#CFLAGS = $(PROFILE_CFLAGS) -O3 -march=native -funroll-loops -ffast-math # Hard optimize for speed

# Libraries
LIBS = -lm -lblas

# Target executables
TARGETS = blas dumb
PROFILE_TARGETS = dumb_profile

# Source files
BLAS_SRCS = main_blas.c 
DUMB_SRCS = main_dumb.c 
EXTRA = utils.h

# Object files
BLAS_OBJS = $(BLAS_SRCS:.c=.o) 
DUMB_OBJS = $(DUMB_SRCS:.c=.o)

# Default target
all: $(TARGETS)

# BLAS target
blas: $(BLAS_OBJS) 
	$(CC) $(CFLAGS) -o blas $(EXTRA) $(BLAS_OBJS) $(LIBS)

# Dumb target
dumb: $(DUMB_OBJS)
	$(CC) $(CFLAGS) -o dumb $(EXTRA) $(DUMB_OBJS) $(LIBS)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(BLAS_OBJS) $(DUMB_OBJS) $(TARGETS) 

clean_profile: 
	rm -rf *.gcda *.gcno *.gcov

# PHONY targets to avoid conflicts with files of the same name
.PHONY: all clean_profile clean

