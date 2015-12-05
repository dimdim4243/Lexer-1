#ifndef LEXOGRAPH_LEXER_H
#define LEXOGRAPH_LEXER_H
#include "Token.h"

class Lexer
{
private:
	int lineCounter;
	int columnCounter;
	char b;
	bool error;
	bool end;
	string lexeme;
	ifstream fin;
	Token* buffer;
	static map<string,Types> typeMap;
public:
	Lexer();
	Lexer(string stream);
	void static ReplaceAll(string *str, string from, string to);
	void SkipWhiteSpaces();
	char NextSym();
	bool static ishex(char b);
	int static stoi (string s);
	int static shtoi (string s);
	double static stor (string s);
	Token* GetToken();
	Token* PopBuffer();
	Token* Error(string code);
};

#endif //LEXOGRAPH_LEXER_H