all: generator sequential parallel correlation result clean

generator:
	python3 generator.py

correlation:
	python3 correlation.py

sequential:
	g++ -std=c++11 sequential.cpp -o sequential.exe; ./sequential.exe > sequential.txt

parallel:
	g++ -std=c++11 parallel.cpp -o parallel.exe; ./parallel.exe > parallel.txt

result:
	python3 result.py

clean:
	rm *.exe sample.txt