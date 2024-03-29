#include "Lexer.h"

map<string, Types> Lexer::typeMap = mapInit();

Lexer::Lexer(){}

Lexer::Lexer(string file):error(false), end(false), lineCounter(1), columnCounter(0), buffer(NULL)
{
    fin.open(file);
    Token::init();
    NextSym();
}

Token* Lexer::PopBuffer()
{
    Token* t = buffer;
    buffer = NULL;
    return t;
}

void Lexer::ReplaceAll(string *str, string from, string to)
{
    unsigned int start_pos = 0;
    while((start_pos = (*str).find(from, start_pos)) != std::string::npos) {
        (*str).replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

void Lexer::SkipWhiteSpaces()
{
    while (b == ' ' || b == '\n' || b == '\t')
    {
        NextSym();
    }
}

char Lexer::NextSym()
{
    if (b == '\t') columnCounter += 4 - (columnCounter - 1) % 4;
    else if (b == '\n')
    {
        lineCounter++;
        columnCounter = 1;
    }
    else columnCounter++;
    fin.get(b);
    if (fin.eof())
    {
        end = true;
        b = '~';
    }
    return b;
}

bool Lexer::ishex(char b)
{
    return b >= '0' && b <= '9' || b >= 'a' && b <= 'f' || b >= 'A' && b <= 'F';
}

int Lexer::stoi(string s)
{
    int b;
    istringstream str(s);
    str >> b;
    return b;
}

int Lexer::shtoi(string s)
{
    int b;
    istringstream str(s);
    str >> hex >> b;
    return b;
}

double Lexer::stor(string s)
{
    float b;
    istringstream str(s);
    str >> b;
    return b;
}

Token* Lexer::Error(string code)
{
    error = true;
    return new TokenError(lineCounter, columnCounter, code);
}


Token* Lexer::GetToken()
{
    SkipWhiteSpaces();
    if (buffer != NULL) return PopBuffer();
    if (end || error) return NULL;
    lexeme = "";
    int currLine = lineCounter, currColumn = columnCounter;
    if (isalpha(b) || b == '_')
    {
        currColumn = columnCounter;
        while (isalnum(b) || b == '_')
        {
            lexeme += b;
            NextSym();
        }
        return new Token(currLine, currColumn, Token::ABTypes(lexeme), lexeme);
    }
    else if (b == '\'')
    {
        int size = 0;
        lexeme += b;
        NextSym();
        while (1)
        {
            if (b == '\n') return Error("BadNL");
            if (end) return Error("BadEOF");
            else if (b == '\'')
            {
                if (NextSym() == '\'')
                {
                    lexeme += string(2, b);
                    size++;
                    NextSym();
                }
                else
                {
                    lexeme += '\'';
                    string value = lexeme.substr(1, lexeme.size() - 2);
                    ReplaceAll(&value, "''", "'");
                    if (size == 1)
                        return new TokenVal<char>(currLine, currColumn, CHARACTER, lexeme, lexeme[1]);
                    else
                        return new TokenVal<string>(currLine, currColumn, STRING, lexeme, value);
                }
            }
            else lexeme += b, size++, NextSym();
        }
    }
    else if(Token::isop(b))
    {
        string buff(1, b);
        NextSym();
        if (Token::isop(buff + b))
        {
            buff += b;
            NextSym();
            return new Token(currLine, currColumn, typeMap[buff], buff);
        }
        else if (buff[0] == '.' && b == '.')
        {
            NextSym();
            return new Token(currLine, currColumn, typeMap[buff + '.'], buff + '.');
        }
        else
        {
            if (buff == ":") return new Token(currLine, currColumn, typeMap[buff], buff);
            else if (buff == "/" && b == '/')
            {
                getline(fin, buff);
                lineCounter++;
                columnCounter = 0;
                NextSym();
                return GetToken();
            }
            return new Token(currLine, currColumn, typeMap[buff], buff);
        }
    }
    else if(Token::issep(b))
    {
        string s(1, b);
        NextSym();
        if (s == "(" && b == '*')
        {
            NextSym();
            while (1)
            {
                if (end)return Error("BadEOF");
                if (b == '*')
                {
                    if (NextSym() == ')')
                    {
                        NextSym();
                        return GetToken();
                    }
                }
                else NextSym();
                SkipWhiteSpaces();
            }
        }
        return new Token(currLine, currColumn, typeMap[s], s);
    }
    else if (b == '{')
    {
        while (1)
        {
            NextSym();
            SkipWhiteSpaces();
            if (end)return Error("BadEOF");
            if (b == '}')
            {
                NextSym(); return GetToken();
            }
        }
    }
    else if (isdigit(b))
    {
        bool r = false, e = false;
        while (isdigit(b) || !r && b == '.' || !e &&  (b == 'e' || b == 'E'))
        {
            if (b == '.')
            {
                r = true;
                if (NextSym() == '.')
                {
                    buffer = new Token(currLine, columnCounter - 1, SEP, "..");
                    NextSym();
                    return new TokenVal<int>(currLine, currColumn, INTEGER, lexeme, stoi(lexeme));
                }
                else if (isdigit(b))
                {
                    lexeme += '.';
                }
                else return Error("NoFract");
            }
            else if (b == 'e' || b == 'E')
            {
                e = true;
                lexeme += b;
                if (NextSym() == '+' || b == '-')
                {
                    lexeme += b;
                    NextSym();
                }
                if (!isdigit(b))
                {
                    return Error("NoExp");
                }
            }
            lexeme += b;
            NextSym();
        }
        if (r || e) return new TokenVal<double>(currLine, currColumn, REAL, lexeme, stor(lexeme));
        else return new TokenVal<int>(currLine, currColumn, INTEGER, lexeme, stoi(lexeme));
    }
    else if (b == '$')
    {
        string h;
        lexeme += b;
        if (ishex(NextSym()))
        {
            while (ishex(b))
            {
                lexeme += b;
                h += b;
                NextSym();
            }
            return new TokenVal<int>(currLine, currColumn, HEX, lexeme, shtoi(h));
        }
        else return Error("NoHex");
    }
    else if (b == '#')
    {
        int c;
        string h;
        lexeme += b;
        if (NextSym() == '$')
        {
            lexeme += b;
            if (ishex(NextSym()))
            {
                while(ishex(b))
                {
                    lexeme += b;
                    h += b;
                    NextSym();
                }
                c = shtoi(h);
            }
            else return Error("NoHex");

        }
        else if(isdigit(b))
        {
            while (isdigit(b))
            {
                lexeme += b;
                h += b;
                NextSym();
            }
            c = stoi(h);
        }
        else return Error("NoCC");
        if (c >= 0 && c <= 127)
        {
            return new TokenVal<char>(currLine, currColumn, CHARACTER, lexeme, (char)c);
        }
        else return Error("BadCC");
    }
    else if (!end) return Error("BadChar");
}