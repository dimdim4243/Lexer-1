#ifndef Keywords
#define Keywords \
kw(BEGIN) kw(FORWARD) kw(DO) kw(ELSE) kw(END) kw(FOR) kw(FUNCTION) kw(IF) kw(ARRAY) kw(OF) kw(PROCEDURE)\
kw(PROGRAM) kw(RECORD) kw(THEN) kw(TO) kw(TYPE) kw(VAR) kw(WHILE) kw(BREAK) kw(CONTINUE) kw(DOWNTO) kw(EXIT)\
kw(REPEAT) kw(UNTIL)
#endif //Keywords

#ifndef Separators
#define Separators \
sep("(", OPENBRAC) sep(")", CLOSEBRAC) \
sep("[", OPENBRACSQ) sep("]", CLOSEBRACSQ) \
sep(";", SEMICOLON) sep(":", COLON) \
sep("..", RANGE) sep(",", COMMA)
#endif //Separators

#ifndef Operators
#define Operators \
optr(AND) optr(DIV) optr(MOD) optr(NOT) optr(OR) optr(XOR) \
\
sop("+", PLUS) sop("-", MINUS) sop("*", MUL) \
sop("/", DIVIS) sop("^", POINTER) sop("-=", MINUSEQ) \
sop("+=", PLUSEQ) sop("*=", MULEQ) sop("/=", DIVISEQ) \
sop(">", BIGGER) sop("<", LESS) sop("<=", LESSEQ) \
sop(">=", BIGGEREQ) sop("=", EQUAL) sop("<>", NOTEQUAL) \
sop(":=", ASSIGN) sop("@", DOG) sop(".", DOT)
#endif //Operators
