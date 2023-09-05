#include "utility/dbgln.hpp"


extern "C" __declspec(dllexport) int Multiply(int a, int b) {
	dbgln("Input is {}, {}", a, b);
	return a * b;
}