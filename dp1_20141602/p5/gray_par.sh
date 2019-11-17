mpicc gray_mpi.c -o gra
mpirun -np 2 -hostfile host gra
mpirun -np 4 -hostfile host gra
mpirun -np 8 -hostfile host gra
mpirun -np 16 -hostfile host gra

rm gra
