// 김형석 2학년 B411063 
#include <iostream>
#include "recta.h"
using namespace std;

Rectangle::Rectangle(int x=0, int y=0, int h=0, int w=0) : xLow(x), yLow(y), height(h), width(w) {}

void Rectangle::Print()
{
	cout << "Position: " << this->xLow << " " << this->yLow  << ";";
	cout << " Height = " << this->height;
	cout << " Width = " << this->width;
	cout << endl;
}

bool Rectangle::SmallerThan(Rectangle& s)
{
	if (height*width < s.height * s.width) return true;
	else return false;
}

bool Rectangle::EqualTo(Rectangle& s)
{
	if (height*width==s.height*s.width) return true;
	else return false;
}

int Rectangle::GetHeight(){ return height; }
int Rectangle::GetWidth(){ return width; }
