cat input

echo "mylock 10"
./mylock 10 < input
echo "mylock 50"
./mylock 50 < input
echo "mylock 100"
./mylock 100 < input
echo "rwlock 10"
./rwlock 10 < input
echo "rwlock 50"
./rwlock 50 < input
echo "rwlock 100"
./rwlock 100 < input

cat input2

echo "mylock 10"
./mylock 10 < input2
echo "mylock 50"
./mylock 50 < input2
echo "mylock 100"
./mylock 100 < input2
echo "rwlock 10"
./rwlock 10 < input2
echo "rwlock 50"
./rwlock 50 < input2
echo "rwlock 100"
./rwlock 100 < input2

cat input3

echo "mylock 10"
./mylock 10 < input3
echo "mylock 50"
./mylock 50 < input3
echo "mylock 100"
./mylock 100 < input3
echo "rwlock 10"
./rwlock 10 < input3
echo "rwlock 50"
./rwlock 50 < input3
echo "rwlock 100"
./rwlock 100 < input3

