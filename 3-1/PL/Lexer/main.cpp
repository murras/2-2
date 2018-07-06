#include <iostream>
#include "Lexer.h"

using namespace std;
int main(int argc, const char* argv[])
{
	/*if (argc == 1)
	{
		cerr << "no argument" << endl
			<< "Usage: Lexer <source file>" << endl;
		return 1;
	}
	Lexer lexer(argv[1]);
	for (Token tok = lexer.next();
		tok.type != Token::eofTok.type;
		tok = lexer.next())
	{
		cout << tok << endl;
	}*/
	Lexer lex(argv[1]);
}