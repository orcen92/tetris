#ifndef __FIGURES_H_
#define __FIGURES_H_

#include <vector>

extern const int ncolors;
enum Colors { RED=0, BLUE, GREEN, MAGENTA, CYAN};


class Rectangle {
	public:
		int xa, xb, ya, yb;
		Rectangle() { ; }
		Rectangle(int xxa, int xxb, int yya, int yyb) : xa(xxa), xb(xxb), ya(yya), yb(yyb) { ; }
//		Rectangle(const Rectangle &r) { xa=r.xa; xb=r.xb; ya=r.ya; yb=r.yb; }
		
};

class Square {
	public:
		int x,y;
		Square() { ; }
		Square(int xx, int yy) : x(xx), y(yy) { ; }
};

class Figure {
	private:
		Colors color;
		std::vector<Square> squares;
		int x, y;


	public:
		enum Direction { DOWN, LEFT, RIGHT};

		static const int nfigures = 7;

		Figure(int type, int xx, int yy);
		~Figure();

		void move(Direction d);
		void rotate(bool clockwise=true); // otherwise anti-clockwis
		Rectangle get_bounds();

		std::vector<std::pair<int, int> > get_squares_positions() const;

		void set_x(int nx) { x = nx; }
		void set_y(int ny) { y = ny; }
		int get_x() { return x; }
		int get_y() { return y; }
	
		Colors get_color() const { return color; }
		unsigned get_color_int() const { return static_cast<unsigned>(color); }
};



#endif
