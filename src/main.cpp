#include "figures.hpp"
#include "game.hpp"
#include "video.hpp"

#include <cstdio>


const int gw = 12;
const int gh = 20;


int main() {

	
	Game game(gw, gh);
	Video video(game.get_grid_width(), game.get_grid_height());

	
	game.run(video);
	

}
