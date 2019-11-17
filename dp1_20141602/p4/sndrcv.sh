mpicc prefix_snd_rcv.c -o prefix_snd_rcv
mpirun -np 2 -hostfile host prefix_snd_rcv 
mpirun -np 4 -hostfile host prefix_snd_rcv 
mpirun -np 8 -hostfile host prefix_snd_rcv 
mpirun -np 16 -hostfile host prefix_snd_rcv 


rm prefix_snd_rcv
