#ifndef __GAME_H_
#define __GAME_H_

#include "figures.hpp"
#include "video.hpp"
#include <list>
#include <SDL2/SDL.h>

class Game {
	private:

	
		static const int clear_delay = 400;
		static const int fall_delay = 10;
		static const int loop_delay = 10;
		static const int move_delay = 10;
		int fall_speed = 1000; // actually 1/speed

		std::list<Square> squares;

		Figure *next_figure = NULL;
		Figure *current_figure = NULL;

		int grid_width, grid_height;
		int score = 0;
		int level = 1;

	public:
		Game(int gw, int gh);
		~Game();

		int get_grid_width() { return grid_width; }
		int get_grid_height() { return grid_height; }

		void move_figure_to_list(Figure &fg);
		
		Figure *new_figure(int type, int x, int y, int rot=0);

		void run(Video &video);

		bool out_of_bounds(const Figure &f);

		bool check_bottom(const Figure &f);
		bool check_sides(const Figure &f, Direction dir);
		const Square * check_touching(const Figure &f, Direction dir);

		const Square * check_overlapping(const Figure &f);

		std::vector<int> check_full_row();
		void clear_row(int row);

		int get_score() { return score; }

		void redraw_all(Video &v, bool figure=true);

//		static void delay(int ms);
		
};


#endif
