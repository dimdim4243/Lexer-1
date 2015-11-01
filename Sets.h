#ifndef LEXOGRAPH_SETS_H
#define LEXOGRAPH_SETS_H

string lower(string a){
	std::transform(a.begin(), a.end(), a.begin(), ::tolower);
	return  a;
}
#define kw(k) s.insert(lower(#k));
#define optr(o)
set<string> set_kws()
{
	set<string> s;
#include "Keywords.h"
	return s;
}
#undef kw
#undef optr
#define kw(k)
#define optr(o) s.insert(lower(#o));
set<string> set_op()
{
	set<string> s;
#include "Keywords.h"
	s.insert("+");
	s.insert("-");
	s.insert("*");
	s.insert("/");
	s.insert("^");
	s.insert("-=");
	s.insert("+=");
	s.insert("*=");
	s.insert("/=");
	s.insert(">");
	s.insert("<");
	s.insert("<=");
	s.insert(">=");
	s.insert("=");
	s.insert("<>");
	s.insert(":=");
	s.insert("@");
	s.insert(".");
	return s;
}
#undef kw
#undef optr

set<string> kws = set_kws();
set<string> ops = set_op();

bool isop(char* b, bool first)
{
	for(set<string>::iterator it = ops.begin(); it != ops.end(); it++)
	{
		if (!isalpha(b[0]) && (first && it.operator*()[0] == b[0])) return true;
		if (!first && it.operator*()[0] == b[0] && it.operator*()[1] == b[1]) return true;
	}
}

#define kw(k) k,
#define optr(t) t,
enum TYPES
{
	_string,
	character,
	integer,
	_hex,
	real,
	ident,
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
		case _string:   return "string";
		case character: return "char";
		case integer:   return "integer";
		case _hex:      return "hex";
		case real:      return "real";
		case op:        return "op";
		case ident:    return "ident";
		case sep:       return "sep";
		case keyword:   return "keyword";
	}
}

#endif
