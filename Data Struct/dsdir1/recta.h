// 2학년 김형석 B411063
#ifndef RECTANGLE_H
#define RECTANGLE_H
class Rectangle{
public:
	Rectangle(int, int, int, int); // constructor
	void Print();
	bool SmallerThan(Rectangle &);
	bool EqualTo(Rectangle &);
	int GetHeight();
	int GetWidth();
private:
	int xLow, yLow, height, width;
};
#endif
	
