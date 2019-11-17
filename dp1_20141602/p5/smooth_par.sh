mpicc smooth_mpi.c -o smo
mpirun -np 2 -hostfile host smo
mpirun -np 4 -hostfile host smo
mpirun -np 8 -hostfile host smo
mpirun -np 16 -hostfile host smo

rm smo
