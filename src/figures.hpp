#ifndef __FIGURES_H_
#define __FIGURES_H_


enum Colors { RED, BLUE, GREEN, MAGENTA, CYAN};



class Square {
	public:
		int x,y;
};

class Figure {
	private:
		Colors color;
		std::vector<Square> squares;
		int x,y;

	public:
		Square(int type);
		~Square();

		rotate(bool clockwise); // otherwise anti-clockwise

};



#endif
