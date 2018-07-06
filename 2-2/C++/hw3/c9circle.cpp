#include <iostream>
#include "c9shape.h"
#include "c9circle.h"
using namespace std;

void Circle::draw() {
	cout << "Circle" << endl;
}

Circle::~Circle() {
	cout << "Circle ¼Ò¸êÀÚ ½ÇÇà" << endl;
}