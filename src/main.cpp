#include "figures.hpp"
#include "game.hpp"
#include "video.hpp"

#include <cstdio>


const int gw = 12;
const int gh = 20;


int main() {


	Game game(gw, gh);
	Video video(game.get_grid_width(), game.get_grid_height());
	std::list<Figure> figures;
	figures.push_back(Figure(6, gw/2, 0));
	
	bool quit = false;
	
	// TODO
	// przenies do game.run()
	// i dodaj cala mechanike gry
	while (not quit) {
		video.draw_background();
		video.draw_figures(figures);
		video.update_window();
		
		SDL_Event e;
		while (SDL_PollEvent(&e)!=0) {
			if (e.type==SDL_QUIT) {
				quit = true;
				break;
			} else if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_e) {
					(figures.begin())->rotate(true);
				} else if (e.key.keysym.sym == SDLK_q) {
					(figures.begin())->rotate(false);
				} else if (e.key.keysym.sym == SDLK_s) {
					(figures.begin())->move(Figure::DOWN);
				} else if (e.key.keysym.sym == SDLK_a) {
					(figures.begin())->move(Figure::LEFT);
				} else if (e.key.keysym.sym == SDLK_d) {
					(figures.begin())->move(Figure::RIGHT);
				}
				game.delay(10);
			}
		}
		game.delay(10);
	}

}
