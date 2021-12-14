all: generator sequential parallel correlation result clean

generator:
	python3 generator.py

correlation:
	python3 correlation.py

sequential:
	g++ -std=c++11 sequential.cpp -o sequential.exe; ./sequential.exe > sequential.txt

parallel:
	g++ -std=c++11 parallel.cpp -lgomp -o parallel.exe; ./parallel.exe > parallel.txt

mpi:
	mpicc -lm mpi.cpp -o mpi.exe; mpirun -np 2 ./mpi.exe

result:
	python3 result.py

clean:
	rm *.exe sample.txt
