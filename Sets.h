#ifndef LEXOGRAPH_SETS_H
#define LEXOGRAPH_SETS_H

string lower(string a){
	std::transform(a.begin(), a.end(), a.begin(), ::tolower);
	return  a;
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

bool isop(char* b, bool first)
{
	for(set<string>::iterator it = ops.begin(); it != ops.end(); it++)
	{
		if (!isalpha(b[0]) && (first && it.operator*()[0] == b[0])) return true;
		if (!first && it.operator*()[0] == b[0] && it.operator*()[1] == b[1]) return true;
	}
}

bool issep(char *b, bool first)
{
	for(set<string>::iterator it = seps.begin(); it != seps.end(); it++)
	{
		if (first && it.operator*()[0] == b[0]) return true;
		if (!first && it.operator*()[0] == b[0] && it.operator*()[1] == b[1]) return true;
	}
}

#define kw(k) k,
#define optr(t) t,
#define sop(s, t) t,
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
};
#undef kw
#undef optr
#undef sop

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
