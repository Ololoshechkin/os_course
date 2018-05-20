#include <iostream>

#include "dynamic/lib_min.h"
#include "RuntimeLibrary.h"
#include "static/lib_print_hello.h"

int main() {
	PrintHello();
	std::cout << "Min(5, 6) = " << Min(5, 6) << std::endl;
	std::cout << "Min(0.7, 0.5, 0.6, 0.55) = " << Min(0.7, 0.5, 0.6, 0.55) << std::endl;
	try {
		RuntimeLibrary lib_max("runtime/runtime_max.dylib");
		auto Max3 = lib_max.GetFunction<int(*)(int, int, int)>("Max3");
		std::cout << "max(5, 6, 3) = " << Max3(5, 6, 3) << std::endl;
	} catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}