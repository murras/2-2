#ifndef LINE_H
#define LINE_H
#include "c9shape.h"

class Line : public Shape {
protected:
	virtual void draw();
public:
	~Line();
};
#endif