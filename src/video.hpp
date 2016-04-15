#ifndef __VIDEO_H_
#define __VIDEO_H_

#include "figures.hpp"

#include <SDL2/SDL.h>
#include <list>

class Video {
	private:

		static const int square_size = 20;
		static const int square_edge = 2;
		static const int space_width = 1;
		static const int hor_margin = 5;
		static const int ver_margin = 5;

		int screen_width, screen_height;

		Uint32 background_color;
		Uint32 frame_color;
		
		std::vector<SDL_Surface*> squares;

		SDL_Window *window;
		SDL_Surface *screen;

		int grid_width, grid_height;

//
		void init_squares();

	public:
		Video(int gw, int gh);
		~Video();

		void update_window();

		void draw_background();
		void draw_figures(std::list<Figure> &fgs);
		void clear_figure(Figure &fg);
		
};

#endif
