#ifndef LEXOGRAPH_SETS_H
#define LEXOGRAPH_SETS_H

#include <set>
#include "Token.h"

using  namespace std;

inline string lower(string a){
	std::transform(a.begin(), a.end(), a.begin(), ::tolower);
	return a;
}
#define kw(k) s.insert(lower(#k));
inline set<string> set_kws()
{
	set<string> s;
#include "Keywords.h"
	return s;
}
#undef kw

#define sep(str, t) s.insert(str);
inline set<string> set_sep()
{
	set<string> s;
#include "Separators.h"
    return s;
}
#undef sep

#define optr(o) s.insert(lower(#o));
#define sop(r, t) s.insert(r);
inline set<string> set_op()
{
	set<string> s;
#include "Operators.h"
	return s;
}
#undef optr
#undef sop



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
inline map<string, Types> mapInit(map<string, Types> m)
{
	#include "Keywords.h"
};
#undef kw
#undef optr
#undef sop


#endif
