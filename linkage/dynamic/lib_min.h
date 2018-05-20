#include <algorithm>

template <typename T>
T Min(T x) {
	return x;
}

template <typename T, typename... Ts>
T Min(T x, Ts... xs) {
	return std::min(x, Min(xs...));
}