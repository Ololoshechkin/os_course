.PHONY: all clean

all: hello_lib.a

hello_lib.a: lib_print_hello.o
	ar rcs hello_lib.a $^
	ranlib hello_lib.a

%.o: %.cpp
	g++ -std=c++17 -c -o $@ $^

clean:
	rm -rf *.o hello_lib.a