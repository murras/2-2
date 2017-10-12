// B411063 2학년 김형석 Kim Hyeong Seok
#include <iostream>
#include <stack>
#include "post.h"
using namespace std;

bool Token::operator==(char b) {
	return len == 1 && str[0] == b;
}
bool Token::operator!=(char b) {
	return len != 1 || str[0] != b;
}

Token::Token() {};
Token::Token(char c) : len(1), type(c) {
	str = new char[1];
	str[0] = c;
}
Token::Token(char c, char c2, int ty) : len(2), type(ty) {
	str = new char[2];
	str[0] = c;
	str[1] = c2;
}
Token::Token(char* arr, int l, int ty = ID) : len(l), type(ty) {
	str = new char[len + 1];
	strcpy(str, arr);
	if (type == NUM) {
		val = str[0] - '0';
		for (int i = 1; i < len; i++)
			val = val * 10 + str[i] - '0';
	}
	else if (type == ID)
		val = 0;
	else
		throw "must be ID or NUM";
}

bool Token::IsOperand() {
	return (type == ID) || (type == NUM);
}

ostream& operator<<(ostream& os, Token t) {
	if (t.type == UMINUS)
		os << "-u";
	else if (t.type == NUM)
		os << t.val;
	else
		for (int i = 0; i < t.len; i++)
			os << t.str[i];
	os << " ";
	return os;
}

bool IsAlpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool IsDigit(char c) {
	return (c >= '0') && (c <= '9');
}

bool GetID(Expression& e, Token& tok) {
	char arr[MAXLEN];
	int idlen;
	char c = e.buf[e.pos];
	if (!IsAlpha(c))
		return false;
	arr[0] = c;
	idlen = 1;
	e.pos++;
	while (IsAlpha(c = e.buf[e.pos]) || IsDigit(c)) {
		arr[idlen++] = c;
		e.pos++;
	}
	arr[idlen] = '\0';
	char *arr2 = new char[idlen + 1];
	strcpy(arr2, arr);
	tok = Token(arr2, idlen, ID);
	return true;
}

bool GetNUM(Expression& e, Token& tok) {
	char arr[MAXLEN];
	int idlen = 1;
	char c = e.buf[e.pos];
	if (!IsDigit(c))
		return false;
	arr[0] = c;
	e.pos++;
	while (IsAlpha(c = e.buf[e.pos]) || IsDigit(c)) {
		arr[idlen++] = c;
		e.pos++;
	}
	arr[idlen] = '\0';
	char *arr2 = new char[idlen + 1];
	strcpy(arr2, arr);
	tok = Token(arr2, idlen, NUM);
	return true;
}

void SkipBlanks(Expression& e) {
	char c;
	while (e.pos < e.len && ((c = e.buf[e.pos]) == ' ' || c == '\t'))
		e.pos++;
}

bool TwoCharOp(Expression& e, Token& tok) {
	char c = e.buf[e.pos];
	char c2 = e.buf[e.pos + 1];
	int op;
	if (c == '=' && c2 == '=') op = EQ;
	else if (c == '!'&&c2 == '=') op = NE;
	else if (c == '>' && c2 == '=') op = GE;
	else if (c == '<'&&c2 == '=') op = LE;
	else if (c == '&' && c2 == '&') op = AND;
	else if (c == '|' && c2 == '|') op = OR;
	else if (c == '-' && c2 == 'u') op = UMINUS;
	else
		return false;
	tok = Token(c, c2, op);
	e.pos += 2;
	return true;
}
bool OneCharOp(Expression& e, Token& tok) {
	char c = e.buf[e.pos];
	if (c == '-' || c == '!' || c == '*' || c == '/' || c == '%' || c == '+' || c == '<' || c == '>' || c == '(' || c == ')' || c == '=') {
		tok = Token(c);
		e.pos++;
		return true;
	}
	return false;
}

Token NextToken(Expression& e) {
	static bool openFound = false;
	Token tok;
	SkipBlanks(e);
	if (e.pos == e.len)
		tok = Token('#'); // 토큰 없음을 알리는 토큰
	else if (GetID(e, tok) || GetNUM(e, tok)) {} // do nothing
	else if (TwoCharOp(e, tok)) {} // do nothing
	else if (OneCharOp(e, tok)) {
		if (e.infix && tok.type == '-' && !openFound)
			tok = Token('-', 'u', UMINUS);
	}
	else throw "illegal Character Found";

	if (e.infix)
		openFound = tok.IsOperand();

	return tok;
}

int icp(Token& t) {
	int ty = t.type;
	if (ty == '(') return 0;
	else if (ty == UMINUS || ty == '!') return 1;
	else if (ty == '*' || ty == '/' || ty == '%') return 2;
	else if (ty == '+' || ty == '-') return 3;
	else if (ty == '>' || ty == '<' || ty == LE || ty == GE) return 4;
	else if (ty == EQ || ty == NE) return 5;
	else if (ty == AND) return 6;
	else if (ty == OR) return 7;
    else if (ty == '=') return 8;
    else if (ty == '#') return 10;
}

int isp(Token& t) {
	int ty = t.type;
	if (ty == '(') return 9;
	else return icp(t);
}

void Postfix(Expression e) {
	stack<Token> sta;
	sta.push(Token('#'));
	for (Token x = NextToken(e); x != '#'; x = NextToken(e)) {
		if (x.IsOperand())
			cout << x;
		else if (x == ')') {
			for (; sta.top() != '('; sta.pop())
				cout << sta.top();
			sta.pop();
		}
		else {
			for (; isp(sta.top()) <= icp(x); sta.pop()) {
				if (x == '='&&sta.top() == '=')
					break;
                cout << sta.top();
			}
			sta.push(x);
		}
	}

	while (sta.top() != '#') {
		cout << sta.top();
		sta.pop();
	}
    sta.pop();
	cout << endl;
}

