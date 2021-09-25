all: generator parallel non_parallel correlation result clean

generator:
	python3 generator.py

correlation:
	python3 correlation.py > correlation.txt

parallel:
	g++ -std=c++11 parallel_main.cpp -o parallel_main.exe; ./parallel_main.exe > parallel_correlation.txt

non_parallel:
	g++ -std=c++11 non_parallel_main.cpp -o non_parallel_main.exe; ./non_parallel_main.exe > non_parallel_correlation.txt

result:
	python3 result.py

clean:
	rm *.exe sample.txt