#ifndef LEXOGRAPH_LEXER_H
#define LEXOGRAPH_LEXER_H

struct TokenBuff
{
	Token* t;
	bool empty;
	TokenBuff();
	TokenBuff(Token* t);
	void push(Token* t);
	Token* pop();
};
TokenBuff::TokenBuff()
{
	empty = true;
}
TokenBuff::TokenBuff(Token* t)
{
	this->t = t;
	empty = false;
}
void TokenBuff::push(Token* t)
{
	this->t = t;
	empty = false;
}
Token* TokenBuff::pop()
{
	empty = true;
	return t;
}

class Lexer
{
private:
	int lineCounter;
	int columnCounter;
	char b;
	bool error;
	bool end;
	string lexeme;
	ifstream fin;
	TokenBuff buffer;
public:
	Lexer() {};
	Lexer(string stream);
	char NextSym();
	bool ishex(char b);
	int static stoi (string s);
	int static shtoi (string s);
	double static stor (string s);
    void ReplaceAll(string *str, string from, string to);
	void SkipWhiteSpaces();
	Token* GetToken();
	Token* Error(string code);
};

void Lexer :: ReplaceAll(string *str, string from, string to)
{
    unsigned int start_pos = 0;
    while((start_pos = (*str).find(from, start_pos)) != std::string::npos) {
        (*str).replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

Lexer :: Lexer(string file):error(false), end(false), lineCounter(1), columnCounter(0)
{
	fin.open(file);
	NextSym();
}

char Lexer :: NextSym()
{
    fin.get(b);
    if (fin.eof())
	{
        end = true;
        b = '~';
    }
    if (b == '\t') columnCounter += 4 - columnCounter % 4;
    else if (b == '\n')
    {
        lineCounter++;
        columnCounter = 0;
    }
    else columnCounter++;
    return b;
}

int Lexer :: stoi(string s)
{
	int b;
	istringstream str(s);
	str >> b;
	return b;
}

double Lexer :: stor(string s)
{
	float b;
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

bool Lexer::ishex(char b)
{
	return b >= '0' && b <= '9' || b >= 'a' && b <= 'f' || b >= 'A' && b <= 'F';
}

void Lexer :: SkipWhiteSpaces()
{
	while (b == ' ' || b == '\n' || b == '\t')
    {
        NextSym();
    }
}

Token* Lexer::Error(string code)
{
	error = true;
	return new TokenError(lineCounter, columnCounter, code);
}

Token* Lexer::GetToken()
{
    SkipWhiteSpaces();
	if (!buffer.empty) return buffer.pop();
	if (end || error) return new Token();
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
		return new Token(currLine, currColumn, ABTypes(lexeme), lexeme);
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
                        return new TokenVal<char>(currLine, currColumn, castType(character), lexeme, lexeme[1]);
                    else
                        return new TokenVal<string>(currLine, currColumn, castType(_string), lexeme, value);
                }
            }
            else lexeme += b, size++, NextSym();
        }
    }
	else if(isop(b))
	{
        string buff(1, b);
        NextSym();
        if (isop(buff + b))
        {
            buff += b;
            NextSym();
            return new Token(currLine, currColumn, castType(op), buff);
        }
        else if (buff[0] == '.' && b == '.')
        {
            NextSym();
            return new Token(currLine, currColumn, castType(sep), buff + '.');
        }
        else
        {
            if (buff == ":") return new Token(currLine, currColumn, castType(sep), buff);
            else if (buff == "/" && b == '/')
            {
                getline(fin, buff);
                lineCounter++;
                columnCounter = 0;
                NextSym();
                return GetToken();
            }
            return new Token(currLine, currColumn, castType(op), buff);
        }
    }
    else if(issep(b))
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
        return new Token(currLine, currColumn, castType(sep), s);
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
		bool r = false;
		lexeme = "";
		while (isdigit(b) || (!r && b == '.' || b == 'e' || b == 'E'))
		{
			if (b == '.')
			{
				r = true;
				if (NextSym() == '.')
				{
					buffer.push(new Token(currLine, columnCounter - 1, castType(sep), ".."));
					NextSym();
					return new TokenVal<int>(currLine, currColumn, castType(integer), lexeme, stoi(lexeme));
				}
				else if (isdigit(b))
				{
					lexeme += '.';
				}
				else return Error("NoFract");
			}
            else if (b == 'e' || b == 'E')
            {
                r = true;
                lexeme += b;
                if (NextSym() == '+' || b == '-')
                {
                    lexeme += b;
                    NextSym();
                    if (!isdigit(b)) return Error("NoExp");
                } else if (!isdigit(b))
                {
                    return Error("NoExp");
                }
            }
			lexeme += b;
			NextSym();
		}
		if (r) return new TokenVal<double>(currLine, currColumn, castType(real), lexeme, stor(lexeme));
		else return new TokenVal<int>(currLine, currColumn, castType(integer), lexeme, stoi(lexeme));
	}
	else if (b == '$')
	{
		string h;
		lexeme += b;
		NextSym();
		if (ishex(b))
		{
			while (ishex(b))
			{
				lexeme += b;
				h += b;
				NextSym();
			}
			return new TokenVal<int>(currLine, currColumn, castType(_hex), lexeme, shtoi(h));
		}
		else return Error("NoHex");
	}
	else if (b == '#')
	{
		int c;
		string h;
		lexeme += b;
		NextSym();
		if (b == '$')
		{
			lexeme += b;
			NextSym();
			if (ishex(b))
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
			return new TokenVal<char>(currLine, currColumn, castType(character), lexeme, (char)c);
		}
		else return Error("BadCC");
	}
	else if (!end) return Error("BadChar");
}

#endif //LEXOGRAPH_LEXER_H