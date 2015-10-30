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
		case _string:   return "string";
		case character: return "char";
		case integer:   return "integer";
		case _hex:      return "hex";
		case real:      return "real";
		case op:        return "op";
		case indent:    return "indent";
		case sep:       return "sep";
		case keyword:   return "keyword";
	}
}

#endif
