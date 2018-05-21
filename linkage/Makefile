.PHONY: all clean

all: program

program: main.o ScopedRuntimeLibrary.o ./static/hello_lib.a ./dynamic/lib_min.so 
	g++ -std=c++17 -L. -l:./static/hello_lib.a -l:./dynamic/lib_min.so -Wl,-rpath,. -o $@ $^ -ldl
	cd runtime && make && cd ..
%.o: %.cpp
	g++ -std=c++17 -c -o $@ $^

./static/hello_lib.a:
	cd static && make && cd ..

./dynamic/lib_min.so:
	cd dynamic && make && cd ..

run:
	./program

clean:
	cd static && make clean && cd ..
	cd dynamic && make clean && cd ..
	cd runtime && make clean && cd ..
	rm -rf *.o program
