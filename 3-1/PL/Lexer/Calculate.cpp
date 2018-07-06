#include "Calculate.h"
#include "TokenType.h"
#include "Token.h"
#include <iostream>
#include <cstring>
using namespace std;

Calculate::Calculate(Lexer* ts){
	lexer = ts;
	token = lexer->next();
	for (int i = 0; i < 26; i++) {
		memory[i] = 0.0;
	}
}
void Calculate::run() {
	bool check = (token.type == TokenType::Eof);
	while (!check){
		// assignment
		double value = assignment();
		check = token.type == TokenType::Eof;
	}
}
string Calculate::match(TokenType t) {
	string val = token.value;
	if (token.type == t) {
		token = lexer->next();
	}
	else {
		error(t);
	}
	return val;
}
void Calculate::error(string msg) {
	cout << msg << '\n';
	exit(1);
}
void Calculate::error(TokenType tok) {
	cout << "Syntax error: expecting: " << tok << "; saw: " << token.value << '\n';
	exit(1);
}
double Calculate::assignment() {
	// Assignment -> Identifier = Expression
	string target = match(TokenType::Identifier);
	char lhs = tolower(target[0]);
	match(TokenType::Assign);
	double source = expression();
	cout << fixed;
	cout.precision(1);
	cout << lhs << " = " << source << '\n';
	memory[lhs - 'a'] = source;
	// cout << "memory[" << (lhs - 'a') << "] = " << memory[lhs - 'a'] << '\n';
	match(TokenType::Semicolon);
	return source;
}
double Calculate::expression() {
	// Expression -> Term { AddOp Term }
	double e = term();
	while (isAddOp()) {
		char op = match(token.type)[0];
		double term2 = term();
		switch(op){
		case '+':e += term2; break;
		case '-':e -= term2; break;
		default: error("illegal operator" + op);
		}
	}
	return e;
}
double Calculate::term() {
	// Term -> Factor { MultiplyOp Factor }
	double e = factor();
	while (isMultiplyOp()) {
		char op = match(token.type)[0];
		double term2 = factor();
		switch(op) {
		case '*': e *= term2; break;
		case '/': e /= term2; break;
		default:
			error("illegal operator" + op);
		}
	}
	return e;
}
double Calculate::factor() {
	// Factor --> [Unary Op] Primary
	if (isUnaryOp()) {
		char op = match(token.type)[0];
		double term = primary();
		return op == '-' ? -term : term;
	}
	else
		return primary();
}
double Calculate::primary() {
	// Primary -> Identifier | Literal | ( Expression )
	string lit = "";
	double e = 0;
	if (token.type == TokenType::Identifier) {
		char rhs = tolower(match(TokenType::Identifier)[0]);
		e = memory[rhs - 'a'];
		// cout << "memory[0] = " << e << '\n';
	}
	else if ((token.type == TokenType::IntLiteral) || (token.type == TokenType::FloatLiteral)) {
		// try 문
		e = stod(match(token.type));
	}
	else if (token.type == TokenType::LeftParen) {
		token = lexer->next();
		e = expression();
		match(TokenType::RightParen);
	}
	else {
		error("Identifier | Number | ( -- " + token.value);
	}
	//cout << "e = " << e << '\n';
	return e;
}

bool Calculate::isAddOp() {
	return token.type == TokenType::Plus || token.type == TokenType::Minus;
}
bool Calculate::isMultiplyOp() {
	return token.type == TokenType::Multiply || token.type == TokenType::Divide;
}
bool Calculate::isUnaryOp() {
	return token.type == TokenType::Not || token.type == TokenType::Minus;
}
