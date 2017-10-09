// B411063 2학년 김형석 Kim Hyeong Seok
#ifndef POSTFIX_H
#define POSTFIX_H

#define ID 257 // 변수 이름이 갖는 타입
#define NUM 258 // 25나 35.7 같은 숫자가 갖는 타입
#define EQ 259 // == 이 갖는 타입
#define NE 260 // !=이 갖는 타입
#define GE 261 // >=이 갖는 타입
#define LE 262 // <=이 갖는 타입
#define AND 263 // && 가 갖는 타입
#define OR 264 // ||이 갖는 타입
#define UMINUS 265 // -u(unary -)가 갖는 타입
//  1자 (연산자) 토큰의 토큰타입 = 그 문자의 아스키코드
#define MAXLEN 80 
#include <cstring> // strlen 사용

struct Expression {
	Expression(char* linebuf, bool inf = false) : buf(linebuf), pos(0), len(strlen(buf)), infix(inf) {}
	char * buf;
	int pos; // Expression 내의 현재의 위치 0~len-1 사이의 수
	int len; // Expression의 buf가 가리키는 문자열의 길이
	bool infix; // true for infix expression ( -처리 위해 필요)
};

struct Token {
	bool operator==(char);
	bool operator!=(char);
	Token();
	Token(char); // 1자 토큰
	Token(char, char, int); // 2자 토큰
	Token(char*, int, int);// operand와 그길이 및 타입(default 는 ID)

	bool IsOperand(); // 토큰 타입이 ID NUM 이면 true
	int type;
	char *str;
	int len;
	float val;
};

#endif
