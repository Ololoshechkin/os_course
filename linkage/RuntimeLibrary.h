#include <dlfcn.h>

#include <stdexcept>
#include <string>

class RuntimeLibrary {
  private:
	void* lib_ptr;
  public:
	RuntimeLibrary(const std::string& path);
	~RuntimeLibrary();
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
