#ifndef __VIDEO_H_
#define __VIDEO_H_

#include "figures.hpp"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <list>

class Video {
	private:

		static const int side_w = 200;
		static const int side_h = 400;
		static const int side_margin = 20;
		static const int small_text_height = 24;
		static const int next_figure_height = 120;
		static const int score_height = 50;
		
		static const int square_size = 25;
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

		static const std::string font_path;
		static const int small_font_size = 24;
		static const int big_font_size = 48;
		TTF_Font *font_small = NULL;
		TTF_Font *font_big = NULL;

		SDL_Rect next_figure_label_rect;
		SDL_Rect next_figure_rect;
		
		SDL_Rect score_rect;
//
		void init_font();
		void init_squares();

	public:
		Video(int gw, int gh);
		~Video();

		void update_window();

		void draw_background();
		void draw_figures(std::list<Figure> &fgs);
		void draw_squares(std::list<Square> &sqs);
		void draw_figure(const Figure &fg);
		void draw_square(const Square &sq);
		
		void draw_next_figure(const Figure &fg);
		
		void clear_figure(Figure &fg);
		
		void render_text(const std::string &text, TTF_Font *font, int x, int y, Uint32 *background=NULL, bool centered=true);
		
		void show_centered_text(const std::string &text);
		void show_pause();
		void show_game_over();
		
		void draw_score(int val);
		
};

#endif
