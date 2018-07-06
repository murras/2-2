#pragma once
#include "Token.h"
#include "Lexer.h"
#include <string>
using namespace std;

class Calculate {
public:
	double memory[26];
	Token token;
	Lexer* lexer;
	
	Calculate() {}
	Calculate(Lexer* ts);
	~Calculate() {}
	void run();
	string match(TokenType t);
	void error(string msg);
	void error(TokenType tok);
	double assignment();
	double expression();
	double term();
	double factor();
	double primary();
	bool isAddOp();
	bool isMultiplyOp();
	bool isUnaryOp();
};