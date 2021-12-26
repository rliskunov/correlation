all: generator sequential openmp correlation result clean

generator:
	python3 generator.py

correlation:
	python3 correlation.py

sequential:
	g++ -std=c++11 sequential.cpp -o sequential.exe; ./sequential.exe > sequential.txt

openmp:
	g++ -std=c++11 openmp.cpp -lgomp -o openmp.exe; ./openmp.exe > openmp.txt

mpi:
	mpicxx mpi.cpp -lm -o mpi.exe

result:
	python3 result.py

clean:
	rm *.exe sample.txt
