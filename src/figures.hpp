#ifndef __FIGURES_H_
#define __FIGURES_H_

#include <vector>

extern const int ncolors;
enum Colors { RED=0, BLUE, GREEN, MAGENTA, CYAN};

enum Direction { DOWN, UP, LEFT, RIGHT};


class Rectangle {
	public:
		int xa, xb, ya, yb;
		Rectangle() { ; }
		Rectangle(int xxa, int xxb, int yya, int yyb) : xa(xxa), xb(xxb), ya(yya), yb(yyb) { ; }
		
};

class Square {
	public:
		int x,y;
		Colors color;
		Square() { ; }
		Square(int xx, int yy, Colors c) : x(xx), y(yy), color(c) { ; }
		Square(const Square &sq) : x(sq.x), y(sq.y), color(sq.color) { ; }
};

class Figure {
	private:
		std::vector<Square> squares;
		int x, y;

	public:

		static const int ntypes = 7;

		Figure();
		Figure(int type, int xx, int yy);
		Figure(const Figure &fg);
		~Figure();
		
		Figure& operator=(const Figure &fg);

		void move(Direction d);
		void rotate(bool clockwise=true); // otherwise anti-clockwise
		Rectangle get_bounds() const;

		std::vector<std::pair<int, int> > get_squares_positions() const;
		std::vector<Square> get_squares(bool absolute = true) const;

		void set_x(int nx) { x = nx; }
		void set_y(int ny) { y = ny; }
		int get_x() { return x; }
		int get_y() { return y; }
		
		bool is_touching(const Square &sq, Direction d) const;

		bool is_overlaping(const Square &sq) const;
};



#endif
