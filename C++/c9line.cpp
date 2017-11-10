#include <iostream>
#include "c9shape.h"
#include "c9line.h"
using namespace std;

void Line::draw() {
	cout << "Line" << endl;
}
Line::~Line() {
	cout << "Line ¼Ò¸êÀÚ ½ÇÇà" << endl;
}