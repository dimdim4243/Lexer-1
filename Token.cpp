#include "Token.h"

Token::Token() {}
set<string> Token::kws = set_kws();
set<string> Token::ops = set_op();
set<string> Token::seps = set_sep();
ofstream Token::fout;
Token::Token(int line, int column, Types type, string lexeme): line(line),
                                                               column(column),
                                                               type(type),
                                                               lexeme(lexeme)
{ }

void Token::PrintToken ()
{
    fout << line << "\t"
    << column << "\t"
    << TypeToString(type) <<"\t"
    << lexeme	<< "\n";
}
void Token::init()
{
    fout.open("output.txt");
}
void TokenError::PrintToken ()
{
    fout << line << "\t"
    << column << "\t"
    << lexeme;
}

TokenError::TokenError(int line, int column, string error)
{
    this->line = line;
    this->column = column;
    this->lexeme = error;
}

Types Token::ABTypes(string word)
{
    if (Token::kws.count(word) != 0)
        return KEYWORD;
    else if (Token::ops.count(word) != 0)
        return OP;
    else
        return IDENT;
}

bool Token::isop(string b)
{
    return ops.count(b);
}

bool Token::isop(char b)
{
    return isop(string(1, b)) || b == ':';
}

bool Token::issep(char b)
{
    return seps.count(string(1, b));
}