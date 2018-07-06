#pragma once
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "TokenType.h"
using namespace std;
class Token {
public:
	Token() {};
	Token(TokenType t, const string& v = "");
	~Token() {};

	static int KEYWORDS;
	static vector<string> reserved;
	static vector<Token> token;

	static Token keyword(const string& name);
	static Token mkIdentTok(const string& name);
	static Token mkIntLiteral(const string& name);
	static Token mkFloatLiteral(const string& name);
	static Token mkCharLiteral(const string& name);

	TokenType type;
	string value;
	
	static Token eofTok;
	static Token boolTok;
	static Token charTok;
	static Token elseTok;
	static Token falseTok;
	static Token floatTok;
	static Token ifTok;
	static Token intTok;
	static Token mainTok;
	static Token trueTok;
	static Token whileTok;
	static Token leftBraceTok;
	static Token rightBraceTok;
	static Token leftBracketTok;
	static Token rightBracketTok;
	static Token leftParenTok;
	static Token rightParenTok;
	static Token semicolonTok;
	static Token commaTok;
	static Token assignTok;
	static Token eqeqTok;
	static Token ltTok;
	static Token lteqTok;
	static Token gtTok;
	static Token gteqTok;
	static Token notTok;
	static Token noteqTok;
	static Token plusTok;
	static Token minusTok;
	static Token multiplyTok;
	static Token divideTok;
	static Token andTok;
	static Token orTok;
};
ostream& operator<<(ostream& os, Token& tok);