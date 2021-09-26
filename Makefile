all: generator cpp correlation result clean

generator:
	python3 generator.py

correlation:
	python3 correlation.py

cpp:
	g++ -std=c++11 main.cpp -o main.exe; ./main.exe > result.txt

result:
	python3 result.py

clean:
	rm *.exe sample.txt