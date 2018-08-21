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

#SBATCH --ntasks=16
#SBATCH --export=NONE
#SBATCH --cpus-per-task=1
#SBATCH --ntasks-per-node=16

srun --export=all -n 16 -N 1 -c 1 --cpu-bind=sockets pat_run ./heat_eq_mpi -m 500 -r 85000 -c 85000
