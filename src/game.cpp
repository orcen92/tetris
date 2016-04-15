#include "game.hpp"


Game::Game(int gw, int gh) :
grid_width(gw),
grid_height(gh)
{

}

Game::~Game() {

}


void Game::delay(int ms) {
	SDL_Delay(ms);
}
