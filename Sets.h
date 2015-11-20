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
	return (bool)ops.count(b);
}

bool isop(char *b)
{
	return isop(string(b)) || *b == ':';
}

bool issep(char *b)
{
    return (bool)seps.count(string(b));
}

#define kw(k) k,
#define optr(t) t,
#define sop(s, t) t,
#define sep(s, t) t,
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
map<string, TYPES> mapInit(map<string, TYPES> m)
{
	#include "Keywords.h"
};
#undef kw
#undef optr
#undef sop

#define kw(k) case k: return #k;
#define optr(t) case t: return #t;
#define sop(s, t) case t: return s;
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
		#include "Keywords.h"
	}
}
#undef kw
#undef optr
#undef sop

#endif
