#include <iostream>
#include "c9circle.h"
#include "c9line.h"
#include "c9rect.h"
#include "c9shape.h"
using namespace std;

int main() {
	Shape *pStart = NULL;
	Shape *pLast;

	pStart = new Circle();
	pLast = pStart;

	pLast = pLast->add(new Rect());
	pLast = pLast->add(new Circle());
	pLast = pLast->add(new Line());
	pLast = pLast->add(new Rect());
	pLast = pLast->add(new Circle());
	pLast = pLast->add(new Line());
	
	Shape *p = pStart;
	while (p != NULL) {
		p->paint();
		p = p->getNext();
	}

	p = pStart;
	while (p != NULL) {
		Shape *q = p->getNext();
		delete p;
		p = q;
	}
}