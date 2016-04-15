#ifndef __VIDEO_H_
#define __VIDEO_H_

#include <random>
#include <ctime>

class Random {
	private:
		
		static std::mt19937 mt;
		
	public:
		
		static int get_int(int n);
		
};

#endif
