all: ./runtime_max.dylib

./runtime_max.dylib: ./runtime_max.o
	g++ -std=c++14 -shared runtime_max.o -o runtime_max.dylib

./runtime_max.o:
	g++ -std=c++14 -fPIC -c *.cpp -o runtime_max.o

clean:
	rm -rf *.dylib *.o
