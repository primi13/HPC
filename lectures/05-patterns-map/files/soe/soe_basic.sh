#!/bin/bash

#SBATCH --job-name=soe_basic
#SBATCH --output=soe_basic.log
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks-per-socket=1
#SBATCH --ntasks-per-core=1
#SBATCH --time=5:00
#SBATCH --mem-per-cpu=2100
#SBATCH --reservation=fri

srun ./soe_basic 2000000000
