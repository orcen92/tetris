#include <iostream>
#include "video.hpp"

#include <utility>

#define HERE(x) std::cout << x << std::endl;

const std::string Video::font_path = std::string("font.ttf");

Video::Video(int gw, int gh) : 
grid_width(gw),
grid_height(gh)
{
	screen_width = 2*hor_margin + grid_width * (space_width + square_size) + space_width + side_margin + side_w; 
	screen_height = 2*ver_margin + grid_height * (space_width + square_size) + space_width;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Tetris",
							  SDL_WINDOWPOS_UNDEFINED,
							  SDL_WINDOWPOS_UNDEFINED,
							  screen_width,
							  screen_height,
							  SDL_WINDOW_SHOWN
	);
	screen = SDL_GetWindowSurface(window);

	background_color = SDL_MapRGB(screen->format, 255, 255, 255);
	frame_color = SDL_MapRGB(screen->format, 0, 0, 0);

	init_squares();

	TTF_Init();
	init_font();
	
	next_figure_label_rect.x = hor_margin + space_width + grid_width * (space_width + square_size) + side_margin;
	next_figure_label_rect.y = ver_margin + space_width + side_margin;
	next_figure_label_rect.w = side_w - side_margin;
	next_figure_label_rect.h = small_text_height;

	next_figure_rect.x = hor_margin + space_width + grid_width * (space_width + square_size) + side_margin;
	next_figure_rect.y = ver_margin + space_width + side_margin*2 + small_text_height;
	next_figure_rect.w = side_w - side_margin;
	next_figure_rect.h = next_figure_height;

	score_rect.x = hor_margin + space_width + grid_width * (space_width + square_size) + side_margin;
	score_rect.y = ver_margin + space_width + side_margin*3 + next_figure_height;
	score_rect.w = side_w - side_margin;
	score_rect.h = score_height;
}

Video::~Video() {
	for (unsigned i=0; i<squares.size(); i++) {
		if (squares[i] != NULL) {
			SDL_FreeSurface(squares[i]);
			squares[i] = NULL;
		}
	}
	SDL_DestroyWindow(window);
	
	if (font_small != NULL) {
		TTF_CloseFont(font_small);
		font_small=NULL;
	}
	if (font_big != NULL) {
		TTF_CloseFont(font_big);
		font_big=NULL;
	}
	TTF_Quit();
	SDL_Quit();
}

void Video::init_font() {
	if (font_small==NULL) {
		font_small = TTF_OpenFont(font_path.c_str(), small_font_size);
	}
	if (font_big==NULL) {
		font_big = TTF_OpenFont(font_path.c_str(), big_font_size);
	}
}

void Video::init_squares() {
	if (squares.empty()) {
		
		// center is color_a, top-left is color_b, bottom-right is color_c
		Uint32 color_a;
//		Uint32 color_b;
//		Uint32 color_c;
		
		for (int i=0; i<ncolors; i++) {
			unsigned r=0, g=0, b=0;
			switch(i) {
				case RED:
					r=230;
					break;
				case BLUE:
					g=51; b=204;
					break;
				case GREEN:
					b=31; g=206;
					break;
				case MAGENTA:
					r=200; b=127;
					break;
				case CYAN:
					g=204; b=255;
					break;
			}
			color_a = SDL_MapRGB(screen->format, r, g, b);
//			color_b = SDL_MapRGB(screen->format, r*0.5, g*0.5, b*0.5);
//			color_c = SDL_MapRGB(screen->format, r*0.2, g*0.2, b*0.2);
			
			// TODO: add edges
			
			SDL_Surface *sf = SDL_CreateRGBSurface(0, square_size, square_size, 32, 0, 0, 0, 0);
			SDL_FillRect(sf, NULL, color_a);
			squares.push_back(sf);
		}
	}
}


void Video::update_window() {
	SDL_UpdateWindowSurface(window);
}

void Video::draw_background() {
	SDL_Rect rect;

	SDL_FillRect(screen, NULL, background_color);

	// draw grids
	for (int i=0; i<grid_height+1; ++i) {
		rect.x = hor_margin;
		rect.w = space_width + grid_width * (space_width + square_size);
		rect.y = ver_margin + ( space_width + square_size) * i;
		rect.h = space_width;
		
		SDL_FillRect(screen, &rect, frame_color);
	}
	for (int i=0; i<grid_width+1; ++i) {
		rect.x = hor_margin + (space_width + square_size) *i;
		rect.y = ver_margin;
		rect.w = space_width;
		rect.h = space_width + grid_height * (space_width + square_size);
		SDL_FillRect(screen, &rect, frame_color);
	}
	
	int x,y;
	// side area
	// "next figure" label
	x = next_figure_label_rect.x + next_figure_label_rect.w/2;
	y = next_figure_label_rect.y + next_figure_label_rect.h/2;

	render_text(std::string("Next figure:"), font_small, x, y, &background_color, true);
	// score label
	
}

void Video::draw_figures(std::list<Figure> &fgs) {
	for (std::list<Figure>::const_iterator it = fgs.begin(); it != fgs.end(); ++it) {
		draw_figure(*it);
	}
}

void Video::draw_figure(const Figure &fg) {

	std::vector<Square> sqs = fg.get_squares();
	for (unsigned i=0; i<sqs.size(); i++) {
		draw_square(sqs[i]);
	}
}

void Video::draw_squares(std::list<Square> &sqs) {
	for (std::list<Square>::const_iterator it = sqs.begin(); it != sqs.end(); ++it) {
		draw_square(*it);
	}
	
}

void Video::draw_square(const Square &sq) {
	SDL_Surface *sq_surf = squares[static_cast<int>(sq.color)];

//	std::cout << sq.color << std::endl;


	SDL_Rect dst;
	dst.x = hor_margin + space_width + (space_width + square_size) * sq.x;
	dst.y = ver_margin + space_width + (space_width + square_size) * sq.y;
	dst.w = square_size;
	dst.h = square_size;

	SDL_BlitSurface(sq_surf, NULL, screen, &dst);
}

void Video::clear_figure(Figure &fg) {

}


void Video::show_game_over() {
	std::string text = "GAME OVER";
	
	// blit on the center of game area
	int x = hor_margin + (space_width + ( space_width + square_size)*grid_width) /2;
	int y = ver_margin + (space_width + ( space_width + square_size)*grid_height)/2;
	
	render_text(text, font_big, x, y, &background_color, true);
	
	update_window();
}


void Video::render_text(std::string text, TTF_Font *font, int x, int y, Uint32 * background, bool centered) {
	SDL_Color black;
	black.r=black.g=black.b=0;

	SDL_Surface *surf = TTF_RenderText_Blended(font, text.c_str(), black);
	
	int w = surf->w;
	int h = surf->h;
	
	SDL_Rect r;
	if (centered) { // center of text surface lies on (x,y)
		r.x = x - w/2;
		r.y = y - h/2;
	} else { // upper-left edge of text surface lies on (x,y)
		r.x = x;
		r.y = y;
	}
	r.w = w;
	r.h = h;
	
	if (background != NULL) {
		SDL_FillRect(screen, &r, *background);
	}
	
	SDL_BlitSurface(surf, NULL, screen, &r);
	SDL_FreeSurface(surf);
}


void Video::draw_next_figure(const Figure &fg) {
	
	Rectangle r = fg.get_bounds();
	int xo = r.xa;
	int yo = r.ya;
	int rw = r.xb - r.xa + 1;
	int rh = r.yb - r.ya + 1;
	
	int sw = (rw+1) * space_width + rw*square_size;
	int sh = (rh+1) * space_width + rh*square_size;
	SDL_Surface *sf = SDL_CreateRGBSurface(0, sw, sh, 32, 0, 0, 0, 0);
	SDL_FillRect(sf, NULL, background_color);
	
	
	std::vector<Square> sqs = fg.get_squares();
	// draw squares
	for (auto it = sqs.begin(); it != sqs.end(); it++) {
		SDL_Surface *sq_surf = squares[static_cast<int>((*it).color)];
		
		// square coordinates, starting from (0,0)
		int sx = (*it).x - xo;
		int sy = (*it).y - yo;
			
		// draw background i.e. frame
		SDL_Rect rdst;
		rdst.x = (space_width + square_size) * sx;
		rdst.y = (space_width + square_size) * sy;
		rdst.w = rdst.h = square_size + 2*space_width;
//		std::cout << rdst.x << " " << rdst.y << " " << rdst.w << " " << rdst.h << std::endl;
		SDL_FillRect(sf, &rdst, SDL_MapRGB(sf->format, 0, 0, 0));
		
		// blit square
		rdst.x = space_width + (space_width + square_size) * sx;
		rdst.y = space_width + (space_width + square_size) * sy;
		rdst.w = rdst.h = square_size;
		SDL_BlitSurface(sq_surf, NULL, sf, &rdst);
	}

	// blit *sf on center of next_figure area
	// center coordinates
	int xc = next_figure_rect.x + next_figure_rect.w/2;
	int yc = next_figure_rect.y + next_figure_rect.h/2;

	SDL_Rect rdst;
	rdst.x = xc - sf->w/2;
	rdst.y = yc - sf->h/2;
	rdst.w = sf->w;
	rdst.h = sf->h;
	
	SDL_BlitSurface(sf, NULL, screen, &rdst);
//	SDL_BlitSurface(sf, NULL, screen, NULL);
	SDL_FreeSurface(sf);

}


void Video::draw_score(int val) {
	std::string text = std::string("Score: ")+std::to_string(val);

	int xc = score_rect.x + score_rect.w/2;
	int yc = score_rect.y + score_rect.h/2;
	
	render_text(text, font_small, xc, yc, &background_color, true);
}

