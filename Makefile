all:
	g++ -Wall bench_memory.cpp -o bench_memory -lrt
clean:
	rm *.o