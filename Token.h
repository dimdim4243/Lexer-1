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

ofstream fout("output.txt");

string ABTypes(string word)
{
    if (kws.count(word) != 0)
        return castType(keyword);
    else if (ops.count(word) != 0)
        return castType(op);
    else
        return castType(indent);
}

class TokenError
{
protected:
    int line;
    int column;
    string type;
public:
    TokenError(){}
    TokenError(int linem, int column, string type);
    void PrintToken();
};

void TokenError :: PrintToken ()
{
    fout << this->line << "\t"
    << this->column << "\t"
    << this->type;
}

TokenError ::TokenError(int linem, int column, string type)
{
    this->line = line;
    this->column = column;
    this->type = type;
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
    void  PrintToken ();
};

Token :: Token(int line, int column, string type, string lexeme)
{
	this->line = line;
	this->column = column;
	this->type = type;
	this->lexeme = lexeme;
}

void Token :: PrintToken ()
{
	fout << this->line << "\t"
	<< this->column << "\t"
	<< this->type << "\t"
	<< this->lexeme	<< "\n";
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
    fout << this->line << "\t"
    << this->column << "\t"
    << this->type << "\t"
    << this->lexeme << "\t"
    << this->value << "\n";
}


#endif
