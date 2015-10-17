#ifndef TOKEN
#define TOKEN
#include <fstream>
#include <string>
#include "stdlib.h"
#include <algorithm>
#include <map>
using namespace std;

ofstream fout("output.txt");

string lower(string a){
    std::transform(a.begin(), a.end(), a.begin(), ::tolower);
    return  a;
}

#define kw(k) m[lower(#k)] = 1;
#define optr(o) m[lower(#o)] = 2;
map<string,int> map_init(){
    map<string,int> m;   
    #include "Keywords.h"
    return m;
}
#undef kw
#undef optr
map<string,int> ABLex = map_init();

#define kw(k) k,
#define optr(t) t,
enum TYPES
{
    _string,
    character,
    integer,
    _hex,
    real,
    indent,
    op,
    sep,
    keyword,
    #include "Keywords.h"
};
#undef kw
#undef optr

string castType (TYPES type)
{
	switch (type)
	{
		case _string:   return "string";    break;
		case character: return "char";      break;
		case integer:   return "integer";   break;
		case _hex:      return "hex";       break;
		case real:      return "real";      break;
		case op:        return "op";        break;
		case indent:    return "indent";    break;
		case sep:       return "sep";       break;
		case keyword:   return "keyword";   break;
	}
}

string ABTypes(string word)
{
    if (ABLex[word] == 1)
        return castType(keyword);
    else if (ABLex[word] == 2)
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
