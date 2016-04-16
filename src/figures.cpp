#include "figures.hpp"
#include "random.hpp"

const int ncolors = 5;

Figure::Figure() {
	x=0;
	y=0;
}

Figure::Figure(int type, int xx, int yy) {
	
	Colors color = static_cast<Colors>(Random::get_int(ncolors));
	
	if (type==0) { // uppercase Gamma (L upside down)
		squares.push_back(Square(1, -1, color));
		squares.push_back(Square(0, -1, color));
		squares.push_back(Square(0, 0, color));
		squares.push_back(Square(0, 1, color));
	} else if (type == 1) { // reversed uppercase Gamma
		squares.push_back(Square(-1, -1, color));
		squares.push_back(Square(0, -1, color));
		squares.push_back(Square(0, 0, color));
		squares.push_back(Square(0, 1, color));
	} else if (type == 2) { // |
		squares.push_back(Square(0, -1, color));
		squares.push_back(Square(0, 0, color));
		squares.push_back(Square(0, 1, color));
		squares.push_back(Square(0, 2, color));
	} else if (type == 3) { // square
		squares.push_back(Square(0, 0, color));
		squares.push_back(Square(0, 1, color));
		squares.push_back(Square(1, 0, color));
		squares.push_back(Square(1, 1, color));
	} else if (type == 4) { // s
		squares.push_back(Square(1, 0, color));
		squares.push_back(Square(0, 0, color));
		squares.push_back(Square(0, 1, color));
		squares.push_back(Square(-1, 1, color));
	} else if (type == 5) { // reversed s
		squares.push_back(Square(-1, 0, color));
		squares.push_back(Square(0, 0, color));
		squares.push_back(Square(0, 1, color));
		squares.push_back(Square(1, 1, color));
	} else if (type == 6) { // T (upside down)
		squares.push_back(Square(0, -1, color));
		squares.push_back(Square(-1, 0, color));
		squares.push_back(Square(0, 0, color));
		squares.push_back(Square(1, 0, color));
	}
	
	x=xx;
	y=yy;

	// position checking moved to Game
	/*
	Rectangle r = get_bounds();
	printf("%d %d %d %d\n", r.xa, r.xb, r.ya, r.yb);
	if (r.xa < 0) x += -r.xa;
	if (r.ya < 0) y += -r.ya;
	*/ 
}

Figure::Figure(const Figure &fg) {
	this->squares = fg.squares;
	this->x = fg.x;
	this->y = fg.y;
}

Figure& Figure::operator=(const Figure &fg) {
	this->squares = fg.squares;
	this->x = fg.x;
	this->y = fg.y;
	return *this;
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
		case UP:
			y--; break;
	}
}

void Figure::rotate(bool clockwise) {
	for (unsigned i=0; i<squares.size(); ++i) {
		int xp = squares[i].x;
		int yp = squares[i].y;
		squares[i].x =  yp * ( clockwise ? -1 : 1);
		squares[i].y = -xp * ( clockwise ? -1 : 1);
	}
}

Rectangle Figure::get_bounds() const {
	int minf = -100;
	int xmin=minf, xmax=minf, ymin=minf, ymax=minf;

	for (unsigned i=0; i<squares.size(); ++i) {
		if ( xmin==minf or squares[i].x < xmin) xmin = squares[i].x;
		if ( xmax==minf or squares[i].x > xmax) xmax = squares[i].x;
		if ( ymin==minf or squares[i].y < ymin) ymin = squares[i].y;
		if ( ymax==minf or squares[i].y > ymax) ymax = squares[i].y;
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

std::vector<Square> Figure::get_squares(bool absolute) const {
	int xo = x, yo = y;
	if ( not absolute ) {
		xo=yo=0;
	}
	std::vector<Square> sqs;
	for (auto it = squares.begin(); it!=squares.end(); ++it) {
		Square sq = *it;
		sq.x += xo;
		sq.y += yo;
		sqs.push_back(sq);
	}
	return sqs;
}


bool Figure::is_touching(const Square &sq, Direction d) const {
	for (auto it = squares.begin(); it!=squares.end(); ++it) {
		switch (d) {
			case UP:
				if ( (sq.x == x+(*it).x and sq.y == y+(*it).y-1)) return true;
				break;
			case DOWN:
				if ( (sq.x == x+(*it).x and sq.y == y+(*it).y+1)) return true;
				break;
			case LEFT:
				if ( (sq.x == x+(*it).x-1 and sq.y == y+(*it).y)) return true;
				break;
			case RIGHT:
				if ( (sq.x == x+(*it).x+1 and sq.y == y+(*it).y)) return true;
				break;
		}
	}
	return false;
	
}

bool Figure::is_overlaping(const Square &sq) const {
	for (auto it = squares.begin(); it!=squares.end(); ++it) {
		if ( sq.x == x+(*it).x and sq.y == y+(*it).y ) return true;
	}
	return false;
}


