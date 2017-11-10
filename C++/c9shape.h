#ifndef SHAPE_H
#define SHAPE_H
class Shape {
	Shape* next;
protected:
	virtual void draw();
public:
	Shape() { next = NULL; }
	virtual ~Shape();
	void paint();
	Shape* add(Shape* p);
	Shape* getNext() { return next; }
};
#endif