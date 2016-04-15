#include "random.hpp"

std::mt19937 Random::mt = std::mt19937(time(0));
		

int Random::get_int(int n) {
	std::uniform_int_distribution<int> d(0, n-1);
	return d(mt);
}
