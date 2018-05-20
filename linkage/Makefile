.PHONY: all clean

all: program

program: main.o ./static/hello_lib.a ./dynamic/lib_min.so .runtime/runtime_max.dylib
	g++ -std=c++17 -L. -l:./static/hello_lib.a -l:./dynamic/lib_min.so -Wl,-rpath,. -o $@ $^ -ldl

%.o: %.cpp
	g++ -std=c++17 -c -o $@ $^

./static/hello_lib.a:
	cd static && make && cd ..

./dynamic/lib_min.so:
	cd dynamic && make && cd ..

.runtime/runtime_max.dylib:
	cd runtime && make && cd ..

clean:
	cd static && make clean && cd ..
	cd dynamic && make clean && cd ..
	cd runtime && make clean && cd ..
	rm -rf *.o program