#include "Token.h"
#include "TokenType.h"

int Token::KEYWORDS = (int)(TokenType::Eof);
vector<string> Token::reserved = vector<string>(Token::KEYWORDS);
vector<Token> Token::token = vector<Token>(Token::KEYWORDS);

Token::Token(TokenType t, const string & v) {
	type = t;
	value = v;
	if (type < TokenType::Eof) {
		int ti = static_cast<int>(t);
		reserved[ti] = v;
		token[ti] = *this;
	}
}
Token Token::keyword(const string & name) {
	char ch = name.at(0);
	if ('A' <= ch && ch <= 'Z')
		return mkIdentTok(name);
	for (int i = 0; i < Token::KEYWORDS; i++) {
		if (name == reserved.at(i))
			return token.at(i);
	}
	return mkIdentTok(name);
}
Token Token::mkIdentTok(const string & name) {
	return Token(TokenType::Identifier, name);
}
Token Token::mkIntLiteral(const string & name) {
	return Token(TokenType::IntLiteral, name);
}
Token Token::mkFloatLiteral(const string & name) {
	return Token(TokenType::FloatLiteral, name);
}
Token Token::mkCharLiteral(const string & name) {
	return Token(TokenType::CharLiteral, name);
}

Token Token::eofTok = Token(TokenType::Eof, "<<EOF>>");
Token Token::boolTok = Token(TokenType::Bool, "bool");
Token Token::charTok = Token(TokenType::Char, "char");
Token Token::elseTok = Token(TokenType::Else, "else");
Token Token::falseTok = Token(TokenType::False, "false");
Token Token::floatTok = Token(TokenType::Float, "float");
Token Token::ifTok = Token(TokenType::If, "if");
Token Token::intTok = Token(TokenType::Int, "int");
Token Token::mainTok = Token(TokenType::Main, "main");
Token Token::trueTok = Token(TokenType::True, "true");
Token Token::whileTok = Token(TokenType::While, "while");
Token Token::leftBraceTok = Token(TokenType::LeftBrace, "{");
Token Token::rightBraceTok = Token(TokenType::RightBrace, "}");
Token Token::leftBracketTok = Token(TokenType::LeftBracket, "[");
Token Token::rightBracketTok = Token(TokenType::RightBracket, "]");
Token Token::leftParenTok = Token(TokenType::LeftParen, "(");
Token Token::rightParenTok = Token(TokenType::RightParen, ")");
Token Token::semicolonTok = Token(TokenType::Semicolon, ";");
Token Token::commaTok = Token(TokenType::Comma, ",");
Token Token::assignTok = Token(TokenType::Assign, "=");
Token Token::eqeqTok = Token(TokenType::Equals, "==");
Token Token::ltTok = Token(TokenType::Less, "<");
Token Token::lteqTok = Token(TokenType::LessEqual, "<=");
Token Token::gtTok = Token(TokenType::Greater, ">");
Token Token::gteqTok = Token(TokenType::GreaterEqual, ">=");
Token Token::notTok = Token(TokenType::Not, "!");
Token Token::noteqTok = Token(TokenType::NotEqual, "!=");
Token Token::plusTok = Token(TokenType::Plus, "+");
Token Token::minusTok = Token(TokenType::Minus, "-");
Token Token::multiplyTok = Token(TokenType::Multiply, "*");
Token Token::divideTok = Token(TokenType::Divide, "/");
Token Token::andTok = Token(TokenType::And, "&&");
Token Token::orTok = Token(TokenType::Or, "||");

ostream& operator<<(ostream& os, Token& tok){
	if (TokenType::Identifier > tok.type) {
		os << tok.value;
	}
	else {
		os << toString[(size_t)tok.type] << '\t' << tok.value;
	}
	return os;
}