#include "lib_min.h"

// explicit:
extern "C" template int Min<int, int>(int a, int b);
extern "C" template int Min<int, int, int>(int a, int b, int c);
extern "C" template double Min<double, double, double, double>(double a, double b, double c, double d);