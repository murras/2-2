#ifndef CIRCLE_H
#define CIRCLE_H
#include "c9shape.h"

class Circle : public Shape {
protected:
	virtual void draw();
public:
	~Circle();
};
#endif