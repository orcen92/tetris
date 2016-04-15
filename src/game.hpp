#ifndef __GAME_H_
#define __GAME_H_

#include <SDL2/SDL.h>

class Game {
	private:

		int grid_width, grid_height;


	public:
		Game(int gw, int gh);
		~Game();

		int get_grid_width() { return grid_width; }
		int get_grid_height() { return grid_height; }

		static void delay(int ms);
		
		
};


#endif
