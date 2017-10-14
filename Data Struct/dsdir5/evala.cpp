//B411063 2학년 김형석
#include <iostream>
#include <stack>
#include "post.h"
using namespace std;
ostream& operator<<(ostream&, Token);
Token NextToken(Expression&);

float GetVal(Token& opnd) {
	if (opnd.type == NUM) return opnd.val;
	return 0; // ID 인경우 일단 0 리턴
}

Token UnaryOp(int op, Token& x) {
	if (!x.IsOperand()) throw "Operand Only for Unary Operator";
	Token tmp; tmp.type = NUM;
	if (op == UMINUS) tmp.val = -GetVal(x);
	else if (op == '!') tmp.val = !GetVal(x);
	else throw "Invalid unary operator";
	return tmp;
}

Token BinaryOp(int op, Token& left, Token& right) {
	//실수값의 경우 %는 허용 안됨
	if (!left.IsOperand() || !right.IsOperand())
		throw "Two Operands required for Binary Operation";
	Token tmp; tmp.type = NUM;
	float val1 = GetVal(left),
		val2 = GetVal(right);
	if (op == '+') tmp.val = val1 + val2;
	else if (op == '-') tmp.val = val1 - val2;
	else if (op == '*') tmp.val = val1 * val2;
	else if (op == '/') tmp.val = val1 / val2;
	else if (op == '<') tmp.val = val1 < val2;
	else if (op == '>') tmp.val = val1 > val2;
	else if (op == GE) tmp.val = val1 >= val2;
	else if (op == LE) tmp.val = val1 <= val2;
	else if (op == EQ) tmp.val = val1 == val2;
	else if (op == NE) tmp.val = val1 != val2;
	else if (op == AND) tmp.val = val1 && val2;
	else if (op == OR) tmp.val = val1 || val2;
	else if (op == '=') throw "Assignment Not Yet Implemented";
	else throw "No such binary operator";
	return tmp;
}

void Eval(Expression e) {
	// postfix 표현식을 입력으로 받아 그 값을 계산한다.
	stack<Token> stack;
	Token opnd1, opnd2;
	for (Token x = NextToken(e); x != '#'; x = NextToken(e)) {
		if (x.IsOperand()) stack.push(x);
		else {
			int op = x.type;
			if (op == UMINUS || op == '!') { // unary operator
				opnd1 = stack.top(); stack.pop();
				stack.push(UnaryOp(op, opnd1));
			}
			else { // binary operator
				opnd2 = stack.top(); stack.pop();
				opnd1 = stack.top(); stack.pop();
				stack.push(BinaryOp(op, opnd1, opnd2));
			}
		}
	}
	cout << stack.top() << endl;
	stack.pop();
}
