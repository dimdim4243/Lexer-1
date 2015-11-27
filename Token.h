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

Types ABTypes(string word)
{
    if (kws.count(word) != 0)
        return KEYWORD;
    else if (ops.count(word) != 0)
        return OP;
    else
        return IDENT;
}

class Token
{
protected:
    int line;
    int column;
    Types type;
    string lexeme;
#define kw(k) case k: return #k;
    static string TypeToString (Types type)
    {
        switch (type)
        {
            case STRING:   return "string";
            case CHARACTER: return "char";
            case INTEGER:   return "integer";
            case HEX:      return "hex";
            case REAL:      return "real";
            case OP:        return "op";
            case IDENT:     return "ident";
            case SEP:       return "sep";
            case KEYWORD:   return "keyword";
#include "Keywords.h"
        }
    }
#undef kw
public:
    Token() {}
    Token(int line, int column, Types type, string lexeme);
    virtual void  PrintToken ();
};


Token :: Token(int line, int column, Types type, string lexeme)
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
	<< TypeToString(type) <<"\t"
	<< lexeme	<< "\n";
}


class TokenError: public Token
{
public:
	TokenError(int line, int column, string error);
	void PrintToken();
};

void TokenError :: PrintToken ()
{
	fout << line << "\t"
	<< column << "\t"
	<< lexeme;
}

TokenError :: TokenError(int line, int column, string error)
{
	this->line = line;
	this->column = column;
	this->lexeme = error;
}

template <typename Value>
class TokenVal: public Token
{
protected:
    Value value;
public:
    TokenVal(int line, int column, Types type, string lexeme, Value value);
    void PrintToken ();
};


template <typename Value>
TokenVal<Value> :: TokenVal(int line, int column, Types type, string lexeme, Value value)
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
    << TypeToString(type) <<"\t"
    << lexeme << "\t"
    << value << "\n";
}

template <>
void TokenVal<double> :: PrintToken()
{
    char buf[11];
    sprintf(buf, "%.4E", value);

    // remove extra digit in exp
    buf[8] = buf[9]; buf[9] = buf[10]; buf[10] = 0;         
    
    fout << line << "\t"
    << column << "\t"
    << TypeToString(type) <<"\t"
    << lexeme << "\t"
    << buf << "\n";
}

#endif
