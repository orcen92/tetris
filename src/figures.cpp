#include "figures.hpp"
#include "random.hpp"

const int ncolors = 5;

Figure::Figure(int type, int xx, int yy) {
	
	color = static_cast<Colors>(Random::get_int(ncolors));
	
	if (type==0) { // uppercase Gamma (L upside down)
		squares.push_back(Square(1, -1));
		squares.push_back(Square(0, -1));
		squares.push_back(Square(0, 0));
		squares.push_back(Square(0, 1));
	} else if (type == 1) { // reversed uppercase Gamma
		squares.push_back(Square(-1, -1));
		squares.push_back(Square(0, -1));
		squares.push_back(Square(0, 0));
		squares.push_back(Square(0, 1));
	} else if (type == 2) { // |
		squares.push_back(Square(0, -1));
		squares.push_back(Square(0, 0));
		squares.push_back(Square(0, 1));
		squares.push_back(Square(0, 2));
	} else if (type == 3) { // square
		squares.push_back(Square(0, 0));
		squares.push_back(Square(0, 1));
		squares.push_back(Square(1, 0));
		squares.push_back(Square(1, 1));
	} else if (type == 4) { // s
		squares.push_back(Square(1, 0));
		squares.push_back(Square(0, 0));
		squares.push_back(Square(0, 1));
		squares.push_back(Square(-1, 1));
	} else if (type == 5) { // reversed s
		squares.push_back(Square(-1, 0));
		squares.push_back(Square(0, 0));
		squares.push_back(Square(0, 1));
		squares.push_back(Square(1, 1));
	} else if (type == 6) { // T (upside down)
		squares.push_back(Square(0, -1));
		squares.push_back(Square(-1, 0));
		squares.push_back(Square(0, 0));
		squares.push_back(Square(1, 0));
	}
	
	x=xx;
	y=yy;
	
	Rectangle r = get_bounds();
	printf("%d %d %d %d\n", r.xa, r.xb, r.ya, r.yb);
	if (r.xa < 0) x += -r.xa;
	if (r.ya < 0) y += -r.ya;
	
}

Figure::~Figure() {
	
}

void Figure::move(Direction d) {
	switch(d) {
		case DOWN:
			y++; break;
		case LEFT:
			x--; break;
		case RIGHT:
			x++; break;
	}
}

void Figure::rotate(bool clockwise) {
	for (unsigned i=0; i<squares.size(); ++i) {
		int xp = squares[i].x;
		int yp = squares[i].y;
		squares[i].x = yp * ( clockwise ? -1 : 1);
		squares[i].y = xp * ( clockwise ? 1 : -1);
	}
}

Rectangle Figure::get_bounds() {
	int xmin=-10, xmax=-10, ymin=-10, ymax=-10;

	for (unsigned i=0; i<squares.size(); ++i) {
		if ( xmin==-10 or squares[i].x < xmin) xmin = squares[i].x;
		if ( xmax==-10 or squares[i].x > xmax) xmax = squares[i].x;
		if ( ymin==-10 or squares[i].y < ymin) ymin = squares[i].y;
		if ( ymax==-10 or squares[i].y > ymax) ymax = squares[i].y;
	}
	
	Rectangle r;
	r.xa = xmin + x;
	r.xb = xmax + x;
	r.ya = ymin + y;
	r.yb = ymax + y;
	
	return r;
}

std::vector<std::pair<int, int> > Figure::get_squares_positions() const {
	std::vector<std::pair<int, int> > ps;
	
	for (unsigned i=0; i<squares.size(); ++i) {
		ps.push_back(std::make_pair(squares[i].x + x, squares[i].y + y));
	}
	return ps;
}

