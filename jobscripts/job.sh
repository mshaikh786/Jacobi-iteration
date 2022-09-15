#!/bin/bash

#SBATCH -n 4 
#SBATCH -p amp

module load gcc/8.3.1 hpcx/2.10.0-mt

mpirun -n 4 ../bin/heat_mpi 
