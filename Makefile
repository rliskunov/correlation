all: generator parallel non_parallel correlation clean

generator:
	python3 generator.py

correlation:
	python3 correlation.py

parallel:
	g++ -std=c++11 parallel_main.cpp -o parallel_main.exe; ./parallel_main.exe

non_parallel:
	g++ -std=c++11 non_parallel_main.cpp -o non_parallel_main.exe; ./non_parallel_main.exe

clean:
	rm *.exe