#!/bin/bash
#===============================================================================
#          FILE:  jobscript.sh
#         USAGE:  ./jobscript.sh 
#   DESCRIPTION:  Each MPI task has enough to allocate the full Magnus node memory.
#                 Using CPU Bindings by sockets to distribute memory bandwidth uniformly
#                 NUMA nodes.
#       OPTIONS:  ---
#  REQUIREMENTS:  ---
#          BUGS:  ---
#         NOTES:  ---
#        AUTHOR:  Mohsin Ahmed Shaikh (MS), mohsin.shaikh@pawsey.org.au
#       COMPANY:  Pawsey Supercomputing Centre
#       VERSION:  1.0
#       CREATED:  01/07/18 16:10:09 AWST
#      REVISION:  ---
#===============================================================================

#SBATCH --account=pawsey0001
#SBATCH --ntasks=28
#SBATCH --time=01:00:00
#SBATCH --export=NONE


module load vtune
module load gcc/5.5.0 openmpi/2.1.2

export PATH=$PATH:../bin

srun --export=all -n 28 -N 1 --cpu-bind=sockets amplxe-cl -c hpc-performance -r results-${SLURM_JOBID} /group/pawsey0001/mshaikh/Application_testsuite/my_codes/mem-stressers/bin/heat_eq_mpi_zeus -m 50 -r 86000 -c 86000
