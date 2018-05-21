#include <dlfcn.h>

#include <cstdlib>

#include "ScopedRuntimeLibrary.h"

ScopedRuntimeLibrary::ScopedRuntimeLibrary(const std::string& path) :
	lib_ptr(dlopen(path.c_str(), RTLD_LAZY)) {
	if (lib_ptr == nullptr) {
		throw std::runtime_error(dlerror());
	}
}

ScopedRuntimeLibrary::~ScopedRuntimeLibrary() {
	dlclose(lib_ptr);
}
