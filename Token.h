#ifndef TOKEN
#define TOKEN
#include <iostream>
#include <fstream>
#include <string>
#include "stdlib.h"
#include <algorithm>
#include <map>
#include <set>
#include <sstream>

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
        return castType(ident);
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

bool Token :: null()
{
	return type.empty();
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
	fout << line << "\t"
	<< column << "\t"
	<< type << "\t"
	<< lexeme	<< "\n";
}


class TokenError: public Token
{
public:
	TokenError(int line, int column, string type);
	void PrintToken();
};

void TokenError :: PrintToken ()
{
	fout << line << "\t"
	<< column << "\t"
	<< type;
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
    fout << line << "\t"
    << column << "\t"
    << type << "\t"
    << lexeme << "\t"
    << value << "\n";
}

template <>
void TokenVal<double> :: PrintToken()
{
    char buf[11];
    sprintf(buf, "%.4e", value);

    // remove extra digit in exp
    buf[8] = buf[9]; buf[9] = buf[10]; buf[10] = 0;         
    
    fout << line << "\t"
    << column << "\t"
    << type << "\t"
    << lexeme << "\t"
    << buf << "\n";
}

#endif
