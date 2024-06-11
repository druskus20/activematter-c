#!/bin/bash -l

#SBATCH -t 1:00:00
#SBATCH -A edu24.DD2356
#SBATCH -p shared


nodes=$1
tasks=$2

#SBATCH --job-name="pedro_birds_$nodes_$tasks"
#SBATCH -e errors-${SLURM_JOB_NUM_NODES}-${SLURM_NTASKS}"


for birds in 128 256 512 1024 2048 4096; do
  touch c_dumb-${SLURM_JOB_NUM_NODES}-${SLURM_NTASKS}-${birds}.txt
  touch c_blas-${SLURM_JOB_NUM_NODES}-${SLURM_NTASKS}-${birds}.txt
  touch c_mpi-${SLURM_JOB_NUM_NODES}-${SLURM_NTASKS}-${birds}.txt
  touch c_omp-${SLURM_JOB_NUM_NODES}-${SLURM_NTASKS}-${birds}.txt

  for i in 1 2 3 4 5; do 
    echo "Running $i c_dumb $birds" >> c_dumb-${SLURM_JOB_NUM_NODES}-${SLURM_NTASKS}-${birds}.txt
    srun ./c_dumb $birds >> c_dumb-${SLURM_JOB_NUM_NODES}-${SLURM_NTASKS}-${birds}.txt
    echo "Running $i c_blas $birds" >> c_blas-${SLURM_JOB_NUM_NODES}-${SLURM_NTASKS}-${birds}.txt
    srun ./c_blas $birds >> c_blas-${SLURM_JOB_NUM_NODES}-${SLURM_NTASKS}-${birds}.txt
    echo "Running $i c_mpi $birds" >> c_mpi-${SLURM_JOB_NUM_NODES}-${SLURM_NTASKS}-${birds}.txt
    srun ./c_mpi $birds  >> c_mpi-${SLURM_JOB_NUM_NODES}-${SLURM_NTASKS}-${birds}.txt
    echo "Running $i c_omp $birds" >> c_omp-${SLURM_JOB_NUM_NODES}-${SLURM_NTASKS}-${birds}.txt
    srun ./c_omp $birds  >> c_omp-${SLURM_JOB_NUM_NODES}-${SLURM_NTASKS}-${birds}.txt
  done
done


