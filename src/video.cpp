#include "video.hpp"

#include <utility>

Video::Video(int gw, int gh) : 
grid_width(gw),
grid_height(gh)
{
	screen_width = 2*hor_margin + grid_width * (space_width + square_size) + space_width;
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

}

Video::~Video() {
	for (unsigned i=0; i<squares.size(); i++) {
		if (squares[i] != NULL) {
			SDL_FreeSurface(squares[i]);
			squares[i] = NULL;
		}
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Video::init_squares() {
	if (squares.empty()) {
		
		// center is color_a, top-left is color_b, bottom-right is color_c
		Uint32 color_a;
		Uint32 color_b;
		Uint32 color_c;
		unsigned r=0, g=0, b=0;
		
		for (int i=0; i<ncolors; i++) {
			switch(i) {
				case RED:
					r=230;
					break;
				case BLUE:
					g=51; b=204;
					break;
				case GREEN:
					r=b=51; g=204;
					break;
				case MAGENTA:
					r=255; g=51; g=153;
					break;
				case CYAN:
					g=204; b=255;
					break;
			}
			color_a = SDL_MapRGB(screen->format, r, g, b);
			color_b = SDL_MapRGB(screen->format, r*0.5, g*0.5, b*0.5);
			color_c = SDL_MapRGB(screen->format, r*0.2, g*0.2, b*0.2);
			
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
}

void Video::draw_figures(std::list<Figure> &fgs) {

//	printf("%u\n", squares.size()); fflush(stdout);

	for (std::list<Figure>::const_iterator it = fgs.begin(); it != fgs.end(); ++it) {
		SDL_Surface *sq = squares[static_cast<int>((*it).get_color())];
		std::vector<std::pair<int,int> > sq_pos = (*it).get_squares_positions();
		for (unsigned i=0; i<sq_pos.size(); i++) {
			int xs = std::get<0>(sq_pos[i]);
			int ys = std::get<1>(sq_pos[i]);
			
			SDL_Rect dst;
			dst.x = hor_margin + space_width + (space_width + square_size) * xs;
			dst.y = ver_margin + space_width + (space_width + square_size) * ys;
			dst.w = square_size;
			dst.h = square_size;
			
//			printf("%d %d %d %d\n", dst.x, dst.y, dst.w, dst.h);
//			fflush(stdout);
			SDL_BlitSurface(sq, NULL, screen, &dst);
		}
	}
}

void Video::clear_figure(Figure &fg) {

}

