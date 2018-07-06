#include <iostream>
#include "c9shape.h"
#include "c9rect.h"

using namespace std;

void Rect::draw() {
	cout << "Rectangle" << endl;
}

Rect::~Rect() {
	cout << "Rect ¼Ò¸êÀÚ ½ÇÇà" << endl;
}