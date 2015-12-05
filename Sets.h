#ifndef LEXOGRAPH_SETS_H
#define LEXOGRAPH_SETS_H

#include <set>
#include "Token.h"
#include "lex_defines.h"

using  namespace std;

inline string lower(string a){
	std::transform(a.begin(), a.end(), a.begin(), ::tolower);
	return a;
}
#define kw(k) s.insert(lower(#k));
inline set<string> set_kws()
{
	set<string> s;
    Keywords
	return s;
}
#undef kw

#define sep(str, t) s.insert(str);
inline set<string> set_sep()
{
	set<string> s;
    Separators
    return s;
}
#undef sep

#define optr(o) s.insert(lower(#o));
#define sop(r, t) s.insert(r);
inline set<string> set_op()
{
	set<string> s;
    Operators
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
    Keywords
    Operators
    Separators
};
#undef kw
#undef optr
#undef sop
#undef sep

#define kw(k) m[#k] = k;
#define optr(k) m[#k] = k;
#define sop(s, t) m[s] = t;
#define sep(s, t) m[s] = t;
inline map<string, Types> mapInit()
{
	map<string, Types> m;
	Keywords
    Operators
    Separators
	return m;
};
#undef kw
#undef optr
#undef sop
#undef sep


#endif
