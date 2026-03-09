#!/bin/bash

#SBATCH --job-name=soe_range
#SBATCH --output=soe_range.log
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks-per-socket=1
#SBATCH --ntasks-per-core=1
#SBATCH --time=5:00
#SBATCH --mem-per-cpu=2100
#SBATCH --reservation=fri
#SBATCH --array=0-3

LOW=(         1  500000001 1000000001 1500000001)
HIGH=(500000000 1000000000 1500000000 2000000000)

srun ./soe_range ${LOW[$SLURM_ARRAY_TASK_ID]} ${HIGH[$SLURM_ARRAY_TASK_ID]}
