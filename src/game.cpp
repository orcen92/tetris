#include "game.hpp"
#include "random.hpp"
#include <iostream>


Game::Game(int gw, int gh) :
grid_width(gw),
grid_height(gh)
{

}

Game::~Game() {
}


//void Game::delay(int ms) {
//	SDL_Delay(ms);
//}


Figure *Game::new_figure(int type, int x, int y, int rot) {
	
	Figure *fg = new Figure(type, x, y);
	for (int i=0; i<rot; i++) {
		fg->rotate(true);
	}
	

	Rectangle r = fg->get_bounds();
	if (r.ya < 0) for (int i=0; i<-r.ya; ++i) fg->move(DOWN);

	return fg;
}

void Game::move_figure_to_list(Figure &fg) {
	std::vector<Square> sqs = fg.get_squares();
	for (std::vector<Square>::iterator it = sqs.begin(); it!=sqs.end(); ++it) {
		squares.push_front(*it);
	}
}

bool Game::check_bottom(const Figure &fg) {
	Rectangle r = fg.get_bounds();
	if (r.yb >= grid_height-1) {
		return true;
	} else {
		return false;
	}
}

const Square* Game::check_touching(const Figure &f, Direction dir) {
	for (auto it = squares.begin(); it != squares.end(); ++it) {
		if (f.is_touching(*it, dir)) {
			return &(*it);
		}
	}
	return NULL;
}

const Square* Game::check_overlapping(const Figure &f) {
	for (auto it = squares.begin(); it != squares.end(); ++it) {
		if (f.is_overlaping(*it)) {
			return &(*it);
		}
	}
	return NULL;
}


std::vector<int> Game::check_full_row() {
	std::vector<int> rows;
	for (int i=0; i<grid_height; i++) {
		int row = i;
		int n=0;
		for (std::list<Square>::const_iterator it = squares.begin(); it!=squares.end(); ++it) {
			if ( (*it).y == row) ++n;
		}
		if (n==grid_width) rows.push_back(row);
	}
	return rows;
}


void Game::clear_row(int row) {
	// remove all squares from that row
	for (auto it = squares.begin(); it!=squares.end(); ) {
		if ((*it).y == row) {
			it=squares.erase(it);
		} else {
			++it;
		}
	}

	// move down all squares lying above that row (lower y)
	for (auto it = squares.begin(); it!=squares.end(); ++it) {
		if ((*it).y < row) {
			(*it).y += 1;
		}
	}
	
}

bool Game::check_sides(const Figure &f, Direction dir) {
	Rectangle r = f.get_bounds();
	if (dir == LEFT  and r.xa <= 0) return true;
	if (dir == RIGHT and r.xb >= grid_width-1) return true;
	
	return false;
}










void Game::run(Video &video) {
	bool quit = false;
	bool game_over = false;
	
	// timer counting to fall_speed
	Uint32 fall_time=0;
	
	while (not quit and not game_over) {
		// create new figures if none
		if (current_figure == NULL) {
			if (next_figure == NULL) {
				current_figure = new_figure(Random::get_int(Figure::ntypes), grid_width/2, 0, Random::get_int(4));
				next_figure = new_figure(Random::get_int(Figure::ntypes), grid_width/2, 0, Random::get_int(4));
			} else {
				current_figure = next_figure;
				next_figure = new_figure(Random::get_int(Figure::ntypes), grid_width/2, 0, Random::get_int(4));
			}
			// check if adding new figure is possible
			if (check_overlapping(*current_figure) != NULL) {
				// if not, the game is over
				delete current_figure;
				current_figure=NULL;
				std::cout << "game over" << std::endl;	
				game_over = true;
				break;
			}
			fall_time = SDL_GetTicks();
		}
		
		// main redraw call
		redraw_all(video);

		bool collision = false;

		int to_rotate=0;
		bool to_fall = false;
		bool to_move = false;
		Direction dir = DOWN;

		// keyboard checking
		SDL_Event e;
		while (SDL_PollEvent(&e)!=0) {
			if (e.type==SDL_QUIT) { // quit
				quit = true;
				break;
			} else if (e.type == SDL_KEYDOWN) {	
				// movement
				if (e.key.keysym.sym == SDLK_SPACE) { // fast fall
					to_fall = true;
				} else if (e.key.keysym.sym == SDLK_e) { // rotate +
						to_rotate = 1;
				} else if (e.key.keysym.sym == SDLK_q) { // rotate -
					to_rotate = -1;
				} else if (e.key.keysym.sym == SDLK_s) { // move down
					to_move = true;
					dir = DOWN;
				} else if (e.key.keysym.sym == SDLK_a) { // move left
					to_move = true;
					dir = LEFT;
				} else if (e.key.keysym.sym == SDLK_d) { // move right
					to_move = true;
					dir = RIGHT;
				} // misc
				else if (e.key.keysym.sym == SDLK_p) {
					// pause
					video.show_pause();
					SDL_Event ee;
					bool p = false;
					// wait for another 'p' press
					while (not p) {
						while (SDL_PollEvent(&ee)!=0) {
							if (ee.type == SDL_KEYDOWN and ee.key.keysym.sym == SDLK_p) {
								p = true;
								break;
							}
						}
						SDL_Delay(loop_delay);
					}
				}
			}
		}
		
		// falls down (space)
		if (to_fall) {
			while (check_bottom(*current_figure) == false and check_touching(*current_figure, DOWN) == NULL) {
				current_figure -> move(DOWN); 
				
				redraw_all(video);				
				SDL_Delay(fall_delay);
			}
			collision = true;
		} else {
			// rotation
			if (to_rotate != 0) {
				bool dir = to_rotate == 1 ? true : false;
				current_figure->rotate(dir);
				bool rotate_back = false;
				
				// check if rotation caused overlaping with a square
				if (check_overlapping(*current_figure) != NULL) 
					rotate_back = true;
				
				// check if rotation caused getting out of bounds
				Rectangle r = current_figure->get_bounds();
				if (r.xa<0 or r.xb>=grid_width or r.ya<0 or r.yb>=grid_height) {
					rotate_back = true;
				}
				
				if (rotate_back) {
					current_figure->rotate(not dir);
				}
			}
			
			// move down, left or right
			if (to_move) {
				bool touching = false;

				// check whether
				if (check_touching(*current_figure, dir) != NULL ) { // touching another figure
					touching = true;
				} else if (dir==DOWN and check_bottom(*current_figure)) { // touching bottom
					touching = true;
				} else if ( (dir==LEFT or dir==RIGHT) and check_sides(*current_figure, dir)) { // touching side walls
					touching = true;
				}
				
				if (not touching) {
					current_figure->move(dir);
					if (dir == DOWN) fall_time = SDL_GetTicks();
				}
			}

			// auto fall
			if (SDL_GetTicks() >= fall_time + fall_speed) {
				// check if movement would cause overlapping or out-of-bounds at the bottom
				if (check_touching(*current_figure, DOWN) != NULL or check_bottom(*current_figure) == true) {
					collision = true;
				} else {
					current_figure->move(DOWN);
					fall_time = SDL_GetTicks();
				}
			}
		}

		// current figure touched bottom or one of lying squares
		if (collision) {
			move_figure_to_list(*current_figure);
			delete current_figure;
			current_figure = NULL;
			
			// check for full rows (goals)
			std::vector<int> full_rows = check_full_row();
			for (std::vector<int>::iterator it = full_rows.begin(); it!=full_rows.end(); ++it) {
				score+=level;
				level = 1+score/5;
//				fall_speed = 1000 * 11./(score+10);
				fall_speed = 100. + 900.*exp(-score/120.);
				std::cout << fall_speed << std::endl;
				clear_row(*it);
				
				redraw_all(video, false);
				SDL_Delay(clear_delay);
			}
		}

		SDL_Delay(loop_delay);
	}
	
	// show game over and wait for quit
	quit = false;
	if (game_over) {
		video.show_game_over();
		
		while (not quit) {
			SDL_Event e;
			while (SDL_PollEvent(&e)!=0) {
				if (e.type==SDL_QUIT or
				    (e.type==SDL_KEYDOWN and e.key.keysym.sym == SDLK_ESCAPE)) {
					quit = true;
					break;
				}
			}
			SDL_Delay(loop_delay);
		}
	}
	
	if (next_figure != NULL) {
		delete next_figure;
	}
	if (current_figure != NULL) { 
		delete current_figure;
	}	
}

void Game::redraw_all(Video &video, bool figure) {
	video.draw_background();
	if (figure)
		video.draw_figure(*current_figure);
	video.draw_squares(squares);
	video.draw_next_figure(*next_figure);
	video.draw_score(score);
	video.update_window();
}
