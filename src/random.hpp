#ifndef __RANDOM_H_
#define __RANDOM_H_

#include <random>
#include <ctime>

class Random {
	private:
		
		static std::mt19937 mt;
		
	public:
		
		static int get_int(int n);
		
};

#endif
