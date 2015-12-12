#ifndef TOKEN
#define TOKEN
#include <iostream>
#include <fstream>
#include <string>
#include "stdlib.h"
#include <algorithm>
#include <map>
#include <sstream>
#include "Sets.h"

using namespace std;

class Token
{
protected:
    static ofstream fout;
public:
    int line;
    int column;
    static string TypeToString (Types type);
    string lexeme;
    Types type;
    static set<string> kws;
    static set<string> ops;
    static set<string> seps;
    static Types ABTypes(string word);
    static bool isop(string b);
    static bool isop(char b);
    static bool issep(char b);
    void static init();
    Token();
    Token(int line, int column, Types type, string lexeme);
    virtual void  PrintToken ();
    virtual string ValToString() {}
};

class TokenError: public Token
{
public:
	TokenError(int line, int column, string error);
	void PrintToken();
};

template <typename Value>
class TokenVal: public Token
{
protected:
    Value value;
public:
    TokenVal(int line, int column, Types type, string lexeme, Value value);
    void PrintToken ();
    string ValToString();
};


template <typename Value>
inline TokenVal<Value>::TokenVal(int line, int column, Types type, string lexeme, Value value)
{
    this->line = line;
    this->column = column;
    this->type = type;
    this->lexeme = lexeme;
    this->value = value;
}

template <typename Value>
inline void TokenVal<Value>::PrintToken()
{
    fout << line << "\t"
    << column << "\t"
    << TypeToString(type) <<"\t"
    << lexeme << "\t"
    << value << "\n";
}

template <>
inline void TokenVal<double>::PrintToken()
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

template <typename Value>
string TokenVal<Value>::ValToString()
{
    stringstream s;
    s << value;
    return s.str();
}

#endif