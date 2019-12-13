gcc -o mylock pth_mylock.c my_rand.c -pthread
gcc -o rwlock pth_rwlock.c my_rand.c -pthread

rm *.o
