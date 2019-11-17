mpicc flip_mpi.c -o fli
mpirun -np 2 -hostfile host fli
mpirun -np 4 -hostfile host fli
mpirun -np 8 -hostfile host fli
mpirun -np 16 -hostfile host fli

rm fli
