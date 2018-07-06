#include <algorithm>
#include <cctype>
#include "Lexer.h"
#include "Calculate.h"

Lexer::Lexer(const string & fileName) {
	input.open(fileName);
	Calculate calc(this);
	calc.run();
}
string Lexer::concat(const string & set) {
	string concat_return;
	do {
		concat_return.push_back(ch);
		ch = nextChar();
	} while (~set.find(ch));
	return concat_return;
}
char Lexer::nextChar() {
	if (ch == eofCH) {
		throw "Attempt to read past end of file";
	}
	col++;
	if (col >= line.size()) {
		getline(input, line);
		if (!input) {
			line.assign(1, eofCH);
		}
		else {
			lineno++;
			line += eolLnCH;
		}
		col = 0;
	}
	return line.at(col);
}
Token Lexer::next() {
	do {
		if (isalpha(ch)) {
			string spelling = concat(letters + digits);
			return Token::keyword(spelling);
		}
		else if (isdigit(ch)) {
			string number = concat(digits);
			if (ch != ' ') {
				return Token::mkIntLiteral(number);
			}
			number += concat(digits);
			return Token::mkFloatLiteral(number);
		}
		else {
			switch (ch) {
			case ' ':
			case '\t':
			case '\r':
			case Lexer::eolLnCH:
				ch = nextChar();
				break;
			case '/':
				ch = nextChar();
				if (ch != '/')
					return Token::divideTok;
				do ch = nextChar();
				while (ch != eolLnCH);
				ch = nextChar();
				break;
			case '\'': {
				char ch1 = nextChar();
				nextChar();
				ch = nextChar();
				return Token::mkCharLiteral(string(1, ch1));
			}
			case Lexer::eofCH: //eofCH
				return Token::eofTok;
			case '+':
				ch = nextChar();
				return Token::plusTok;
			case '-':ch = nextChar();
				return Token::minusTok;
			case '*':
				ch = nextChar();
				return Token::multiplyTok;
			case '(':
				ch = nextChar();
				return Token::leftParenTok;
			case ')':
				ch = nextChar();
				return Token::rightParenTok;
			case '{':
				ch = nextChar();
				return Token::leftBraceTok;
			case '}':
				ch = nextChar();
				return Token::rightBraceTok;
			case ';':
				ch = nextChar();
				return Token::semicolonTok;
			case ',':
				ch = nextChar();
				return Token::commaTok;
			case '&':
				check('&');
				return Token::andTok;
			case '|':
				check('|');
				return Token::orTok;
			case '=':
				return chkOpt('=', Token::assignTok, Token::eqeqTok);
			case '<':
				return chkOpt('<', Token::ltTok, Token::lteqTok); // = 
			case '>':
				return chkOpt('>', Token::gtTok, Token::gteqTok); // =
			case '!':
				return chkOpt('!', Token::notTok, Token::noteqTok); // =
			default:
				throw "undefined character"; break;
			}
		}
	} while (true);
}
void Lexer::check(const char c) {
	ch = nextChar();
	if (ch != c) {
		string err = string("Illegal character, expecting ") + c;
		throw err.c_str();
	}
	ch = nextChar();
}
Token Lexer::chkOpt(char c, const Token & one, const Token & two) {
	ch = nextChar();
	if (ch != c) {
		return one;
	}
	ch = nextChar();
	return two;
}
