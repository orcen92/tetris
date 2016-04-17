#ifndef __GAME_H_
#define __GAME_H_

#include "figures.hpp"
#include "video.hpp"
#include <list>
#include <SDL2/SDL.h>

class Game {
	private:
	
		// SDL_Delay constants
		static const int clear_delay = 400;
		static const int fall_delay = 10;
		static const int loop_delay = 10;
		static const int move_delay = 10;
		int fall_speed = 1000; // actually 1/speed

		// list of squares lying on the bottom
		std::list<Square> squares;

		// current and next figures
		Figure *next_figure = NULL;
		Figure *current_figure = NULL;

		int grid_width, grid_height;
		int score = 0;
		int level = 1;

	public:
		// constructor, destructor
		Game(int gw, int gh);
		~Game();
		// main loop
		void run(Video &video);

		int get_grid_width() { return grid_width; }
		int get_grid_height() { return grid_height; }

		// removes figure and adds its squares to square list
		void move_figure_to_list(Figure &fg);
		
		// creates new figure (by `new`) and ensures that it fits the grid
		Figure *new_figure(int type, int x, int y, int rot=0);

		// checks if the figure is right over the bottom
		bool check_bottom(const Figure &f);
		// checks if the figure is touching one of the side walls
		bool check_sides(const Figure &f, Direction dir);
		// checks if the figure touches one of the squares in given direction
		const Square * check_touching(const Figure &f, Direction dir);
		// checks if the figure overlaps with one of the squares
		const Square * check_overlapping(const Figure &f);
		
		// check if there are any full rows and returns array of their indices
		std::vector<int> check_full_row();
		// removes squares from full row and moves all squares above down
		void clear_row(int row);

		int get_score() { return score; }
		
		// redraws screen (may be not with current figure)
		void redraw_all(Video &v, bool figure=true);
		
};


#endif
