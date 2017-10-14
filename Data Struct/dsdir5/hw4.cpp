// B411063 2학년 김형석 Kim Hyeong Seok
#include <iostream>
#include "post.h"
using namespace std;

void Postfix(Expression);

int main() {
	char linebuf[MAXLEN];
	while (cin.getline(linebuf, MAXLEN)) {
		Expression e(linebuf, true);
		// 줄 버퍼 이용 Expression을 만듬 true는 infix식을 의미
		try { Postfix(e); }
		catch (char const *msg) {
			cerr << "Exception: " << msg << endl;
		}
	}
}
