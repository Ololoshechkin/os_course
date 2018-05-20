#include <algorithm>

template <typename T>
T Max(T x) {
	return x;
}

template <typename T, typename... Ts>
T Max(T x, Ts... xs) {
	return std::max(x, Max(xs...));
}


extern "C" int Max3(int a, int b, int c);