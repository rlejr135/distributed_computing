mpicc prefix_scan.c -o prefix_scan
mpirun -np 2 -hostfile host prefix_scan
mpirun -np 4 -hostfile host prefix_scan
mpirun -np 8 -hostfile host prefix_scan
mpirun -np 16 -hostfile host prefix_scan



rm prefix_scan
