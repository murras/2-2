#ifndef RECT_H
#define RECT_H
#include "c9shape.h"

class Rect : public Shape {
protected:
	virtual void draw();
public:
	~Rect();
};
#endif