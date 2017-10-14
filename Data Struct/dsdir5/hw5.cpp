// B411063 2학년 김형석
#include <iostream>
#include "post.h"
using namespace std;
void Eval(Expression);

int main(void) {
	char line[MAXLEN];
	while (cin.getline(line, MAXLEN)) {
		Expression e(line); // Assume postfix notation
		try {
			Eval(e);
		}
		catch (char const *msg) {
			cerr << "Exception: " << msg << endl;
		}
	}
}

