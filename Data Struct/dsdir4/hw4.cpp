// B411063 2�г� ������ Kim Hyeong Seok
#include <iostream>
#include "post.h"
using namespace std;

void Postfix(Expression);

int main() {
	char linebuf[MAXLEN];
	while (cin.getline(linebuf, MAXLEN)) {
		Expression e(linebuf, true);
		// �� ���� �̿� Expression�� ���� true�� infix���� �ǹ�
		try { Postfix(e); }
		catch (char const *msg) {
			cerr << "Exception: " << msg << endl;
		}
	}
}
