.PHONY: all clean

all: lib_min.so

%.o: %.cpp
	g++ -std=c++17 -fPIC -c -o $@ $^

lib_min.so: lib_min.o
	g++ -std=c++17 -shared -o lib_min.so $^

clean:
	rm -rf lib_min.so *.o