// 김형석 2학년 B411063 
#include <iostream>
#include "rectb.h"
using namespace std;

Rectangle::Rectangle(int x=0, int y=0, int h=0, int w=0) : xLow(x), yLow(y), height(h), width(w) {}

ostream& operator<< (ostream& os, Rectangle& s)
{
	os << "Position: " << s.xLow << " " << s.yLow << "; "
		<< "Height = " << s.GetHeight()
		<< " Width = " << s.GetWidth() << endl;
	return os;
}

bool Rectangle::operator< (Rectangle& s)
{
	if ((this->height*this->width) < (s.height*s.width) )
		return true;
	else return false;
}

bool Rectangle::operator== (Rectangle& s)
{
	if ((this->height*this->width) == (s.GetHeight()*s.GetHeight()) )
		return true;
	else return false;		
}
int Rectangle::GetHeight(){ return height; }
int Rectangle::GetWidth(){ return width; }
