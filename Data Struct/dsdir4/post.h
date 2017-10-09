// B411063 2�г� ������ Kim Hyeong Seok
#ifndef POSTFIX_H
#define POSTFIX_H

#define ID 257 // ���� �̸��� ���� Ÿ��
#define NUM 258 // 25�� 35.7 ���� ���ڰ� ���� Ÿ��
#define EQ 259 // == �� ���� Ÿ��
#define NE 260 // !=�� ���� Ÿ��
#define GE 261 // >=�� ���� Ÿ��
#define LE 262 // <=�� ���� Ÿ��
#define AND 263 // && �� ���� Ÿ��
#define OR 264 // ||�� ���� Ÿ��
#define UMINUS 265 // -u(unary -)�� ���� Ÿ��
//  1�� (������) ��ū�� ��ūŸ�� = �� ������ �ƽ�Ű�ڵ�
#define MAXLEN 80 
#include <cstring> // strlen ���

struct Expression {
	Expression(char* linebuf, bool inf = false) : buf(linebuf), pos(0), len(strlen(buf)), infix(inf) {}
	char * buf;
	int pos; // Expression ���� ������ ��ġ 0~len-1 ������ ��
	int len; // Expression�� buf�� ����Ű�� ���ڿ��� ����
	bool infix; // true for infix expression ( -ó�� ���� �ʿ�)
};

struct Token {
	bool operator==(char);
	bool operator!=(char);
	Token();
	Token(char); // 1�� ��ū
	Token(char, char, int); // 2�� ��ū
	Token(char*, int, int);// operand�� �ױ��� �� Ÿ��(default �� ID)

	bool IsOperand(); // ��ū Ÿ���� ID NUM �̸� true
	int type;
	char *str;
	int len;
	float val;
};

#endif
