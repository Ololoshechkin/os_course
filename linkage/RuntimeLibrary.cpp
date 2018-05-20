
#include <cstdlib>
#include <stdexcept>

#include "RuntimeLibrary.h"

RuntimeLibrary::RuntimeLibrary(const std::string& path) :
	lib_ptr(dlopen(path.c_str(), RTLD_LAZY)) {
	if (lib_ptr == nullptr) {
		throw std::runtime_error(dlerror());
	}
}

RuntimeLibrary::~RuntimeLibrary() {
	dlclose(lib_ptr);
}