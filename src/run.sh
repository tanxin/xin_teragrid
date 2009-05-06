#PBS -l nodes=32:ppn=1:fastcpu
mpirun -np 8 -machinefile $PBS_NODEFILE Documents/src/a.out
