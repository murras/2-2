#include <iostream>
#include "c9shape.h"
using namespace std;

void Shape::paint() {
	draw(); // ���� ���ε�
}

void Shape::draw() {
	cout << "--Shape--" << endl;
}


Shape* Shape::add(Shape* p) {
	this->next = p;
	return p;
}

Shape::~Shape() {
	cout << " shape �Ҹ��� ����" << endl;
}