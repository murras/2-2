#include <iostream>
#include "c9shape.h"
using namespace std;

void Shape::paint() {
	draw(); // 동적 바인딩
}

void Shape::draw() {
	cout << "--Shape--" << endl;
}


Shape* Shape::add(Shape* p) {
	this->next = p;
	return p;
}

Shape::~Shape() {
	cout << " shape 소멸자 실행" << endl;
}