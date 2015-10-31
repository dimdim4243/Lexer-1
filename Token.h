#ifndef TOKEN
#define TOKEN
#include <iostream>
#include <fstream>
#include <string>
#include "stdlib.h"
#include <algorithm>
#include <map>
#include <set>
#include <iomanip>

using namespace std;

#include "Sets.h"

string ABTypes(string word)
{
    if (kws.count(word) != 0)
        return castType(keyword);
    else if (ops.count(word) != 0)
        return castType(op);
    else
        return castType(indent);
}

class Token
{
protected:
    int line;
    int column;
    string type;
    string lexeme;
public:
    Token() {}
    Token(int line, int column, string type, string lexeme);
    virtual void  PrintToken ();
	bool null();
};

bool Token::null()
{
	return this->type.empty();
}

Token :: Token(int line, int column, string type, string lexeme)
{
	this->line = line;
	this->column = column;
	this->type = type;
	this->lexeme = lexeme;
}

void Token :: PrintToken ()
{
	ofstream fout("output.txt", ios_base::app);
	fout << fixed << setprecision(10);
	fout << this->line << "\t"
	<< this->column << "\t"
	<< this->type << "\t"
	<< this->lexeme	<< "\n";
}


class TokenError: public Token
{
public:
	TokenError(int line, int column, string type);
	void PrintToken();
};

void TokenError :: PrintToken ()
{

	ofstream fout("output.txt", ios_base::app);
	fout << fixed << setprecision(10);
	fout << this->line << "\t"
	<< this->column << "\t"
	<< this->type;
}

TokenError :: TokenError(int line, int column, string type)
{
	this->line = line;
	this->column = column;
	this->type = type;
}

template <typename Value>
class TokenVal: public Token
{
protected:
    Value value;
public:
    TokenVal(int line, int column, string type, string lexeme, Value value);
    void PrintToken ();
};


template <typename Value>
TokenVal<Value> :: TokenVal(int line, int column, string type, string lexeme, Value value)
{
    this->line = line;
    this->column = column;
    this->type = type;
    this->lexeme = lexeme;
    this->value = value;
}

template <typename Value>
void TokenVal<Value> :: PrintToken()
{
	ofstream fout("output.txt", ios_base::app);
	fout << fixed << setprecision(10);
    fout << this->line << "\t"
    << this->column << "\t"
    << this->type << "\t"
    << this->lexeme << "\t"
    << this->value << "\n";
}


#endif
