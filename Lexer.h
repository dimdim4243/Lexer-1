#ifndef LEXOGRAPH_LEXER_H
#define LEXOGRAPH_LEXER_H
#define wneof(a) while((a) && !fin.eof())

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
	int currColumn;
	char b;
	bool error;
	bool end;
	string lexeme;
	ifstream fin;
	TokenBuff buffer;
public:
	Lexer() {};
	Lexer(string stream);
	void NextSym();
	bool ishex(char b);
	int static stoi (string s);
	int static shtoi (string s);
	double static stor (string s);
	void CountLaC();
	Token* GetToken();
	Token* Error(string code);
};

Lexer::Lexer(string file)
{
	error = false;
	end = false;
	fin.open(file);
	lineCounter = 1;
	columnCounter = 0;
	currColumn = 0;
	NextSym();
}

void Lexer::NextSym()
{
	fin.get(b);
    columnCounter++;
    if (fin.eof())
	{
		end = true;
		b = '~';
	}
}

int Lexer::stoi(string s)
{
	int b;
	istringstream str(s);
	str >> b;
	return b;
}

double Lexer::stor(string s)
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

void Lexer::CountLaC()
{
	if (b == ' ')
	{
		NextSym();
	}
	else if (b == '\t')
	{
		columnCounter += 4 - (columnCounter - 1) % 4;
		NextSym();
	}
	else if (b == '\n')
	{
		lineCounter++;
		columnCounter = 0;
		NextSym();
	}
    else return;
    CountLaC();
}

Token* Lexer::Error(string code)
{
	error = true;
	return new TokenError(lineCounter, columnCounter, code);
}

Token* Lexer::GetToken()
{
    CountLaC();
	if (!buffer.empty) return buffer.pop();
    if (end || error) return new Token();
	lexeme = "";
	currColumn = columnCounter;
	if (isalpha(b) || b == '_')
	{
		currColumn = columnCounter;
		while (isalnum(b) || b == '_')
		{
			lexeme += b;
			NextSym();
		}
		return new Token(lineCounter, currColumn, ABTypes(lexeme), lexeme);
	}
    else if (b == '\'')
    {
        int size = 0;
        lexeme += b;
        NextSym();
        while (b != '\'')
        {
            if (b == '\n') return Error("BadNL");
            if (end) return Error("BadEOF");
            lexeme += b; size++;
            NextSym();
        }
        lexeme += b;
        NextSym();
        if (size == 1)
            return new TokenVal<char>(lineCounter, currColumn, castType(character), lexeme, lexeme[1]);
        else
            return new TokenVal<string>(lineCounter, currColumn, castType(_string), lexeme, lexeme.substr(1, lexeme.size() - 2));
    }
	else if(isop(b))
	{
        string buff(1, b);
        NextSym();
        if (isop(buff + b))
        {
            buff += b;
            NextSym();
            return new Token(lineCounter, currColumn, castType(op), buff);
        }
        else if (buff[0] == '.' && b == '.')
        {
            NextSym();
            return new Token(lineCounter, currColumn, castType(sep), buff + '.');
        }
        else
        {
            if (buff == ":") return new Token(lineCounter, currColumn, castType(sep), buff);
            return new Token(lineCounter, currColumn, castType(op), buff);
        }
    }
    else if(issep(b))
    {
        string s(&b);
        NextSym();
        return new Token(lineCounter, currColumn, castType(sep), s);
    }
	else if (isdigit(b))
	{
		bool r = 0;
		lexeme = "";
		while (isdigit(b) || (!r && b == '.'))
		{
			if (b == '.')
			{
				r = 1;
				NextSym();
				if (b == '.')
				{
					Token twoDots(lineCounter, columnCounter - 1, castType(sep), "..");
					buffer.push(&twoDots);
					NextSym();
					return new TokenVal<int>(lineCounter, currColumn, castType(integer), lexeme, stoi(lexeme));
				}
				else if (isdigit(b))
				{
					lexeme += '.';
				}
				else return Error("NoFract");
			}
			lexeme += b;
			NextSym();
		}
		if (r) return new TokenVal<double>(lineCounter, currColumn, castType(real), lexeme, stor(lexeme));
		else return new TokenVal<int>(lineCounter, currColumn, castType(integer), lexeme, stoi(lexeme));
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
			return new TokenVal<int>(lineCounter, currColumn, castType(_hex), lexeme, shtoi(h));
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
			return new TokenVal<char>(lineCounter, currColumn, castType(character), lexeme, (char)c);
		}
		else return Error("BadCC");
	}
	else if (!end) return Error("BadChar");
}

#endif //LEXOGRAPH_LEXER_H
