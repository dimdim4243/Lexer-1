#ifndef LEXOGRAPH_SETS_H
#define LEXOGRAPH_SETS_H

string lower(string a){
	std::transform(a.begin(), a.end(), a.begin(), ::tolower);
	return a;
}
#define kw(k) s.insert(lower(#k));
set<string> set_kws()
{
	set<string> s;
#include "Keywords.h"
	return s;
}
#undef kw


#define sep(str, t) s.insert(str);
set<string> set_sep()
{
	set<string> s;
#include "Separators.h"
    return s;
}
#undef sep

#define optr(o) s.insert(lower(#o));
#define sop(r, t) s.insert(r);
set<string> set_op()
{
	set<string> s;
#include "Operators.h"
	return s;
}
#undef optr
#undef sop

set<string> kws = set_kws();
set<string> ops = set_op();
set<string> seps = set_sep();

bool isop(string b)
{
	return ops.count(b);
}

bool isop(char b)
{
	return isop(string(1, b)) || b == ':';
}

bool issep(char b)
{
    return seps.count(string(1, b));
}

#define kw(k) k,
#define optr(t) t,
#define sop(s, t) t,
#define sep(s, t) t,
enum Types
{
	STRING,
	CHARACTER,
	INTEGER,
	HEX,
	REAL,
	IDENT,
	OP,
	SEP,
	KEYWORD,
#include "Keywords.h"
#include "Operators.h"
#include "Separators.h"
};
#undef kw
#undef optr
#undef sop
#undef sep

#define kw(k) m[#k] = k;
#define optr(t) m[#t] = t;
#define sop(s, t) m[#s] = t;
map<string, Types> mapInit(map<string, Types> m)
{
	#include "Keywords.h"
};
#undef kw
#undef optr
#undef sop

#define kw(k) case k: return #k;
#define optr(t) case t: return #t;
#define sop(s, t) case t: return s;
string castType (Types type)
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
#undef optr
#undef sop

#endif
