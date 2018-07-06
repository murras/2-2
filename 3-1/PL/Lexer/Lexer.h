#pragma once
#include <string>
#include <fstream>
#include "Token.h"
#include <string>
using namespace std;
class Lexer {
public:
	Lexer() {};
	Lexer(const string& fileName);
	~Lexer() {};
	const string letters = string("abcdefghijklmnopqrstuvwxyz") + string("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	const string digits = "0123456789";
	static constexpr char eofCH = 0x4;
	static constexpr char eolLnCH = '\n';
	Token next();
	string concat(const string& set);
	char nextChar();
	void check(const char c);
	Token chkOpt(char c, const Token& one, const Token& two);
	ifstream input;
	char ch = ' ';
	string line = "";
	int lineno = 0;
	int col = 1;
};