#ifndef scoped_run_lib
#define scoped_run_lib

#include <dlfcn.h>

#include <stdexcept>
#include <string>

class ScopedRuntimeLibrary {
  private:
	void* lib_ptr;
  public:
	ScopedRuntimeLibrary(const std::string& path);
	~ScopedRuntimeLibrary();
	template<typename Function>
	Function GetFunction(const std::string& name) {
		auto func_ptr = dlsym(lib_ptr, name.c_str());
		if (func_ptr == nullptr) {
			std::string error_message = dlerror();
			dlclose(lib_ptr);
			throw std::runtime_error(error_message);
		}
		return (Function)func_ptr;
	}
};

#endif