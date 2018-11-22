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
#SBATCH --nodes=4
#SBATCH --export=NONE
if [ "${PAWSEY_CLUSTER}" == "chaos" ];then
    module load craype-haswell
fi

export PATH=$PATH:../bin
srun --export=all -n 96 -N 4 -c 1 --cpu-bind=sockets heat_eq_mpi -m 10 -r 172000 -c 172000
