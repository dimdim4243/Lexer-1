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
	Token* Error(string code, int col);
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
	if (!fin.eof()) fin.get(b);
	if (b != '\n') columnCounter++;
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
		CountLaC();
	}
	else if (b == '\t')
	{
		columnCounter += 4 - (columnCounter - 1) % 4;
		NextSym();
		CountLaC();
	}
	else if (b == '\n')
	{
		lineCounter++;
		columnCounter = 0;
		NextSym();
		CountLaC();
	}
}

Token* Lexer::Error(string code, int col)
{
	error = true;
	return new TokenError(lineCounter, col, code);
}

Token* Lexer::GetToken()
{
	if (!buffer.empty) return buffer.pop();
	if (end || error) return new Token();
	CountLaC();
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
	else if(isop(&b))
	{
        string buff;
        string f(&b);
        buff += b;
        NextSym();
        if (!end) buff += b;
        if (isop(buff))
        {
            NextSym();
            return new Token(lineCounter, currColumn, castType(op), buff);
        }
        else if (buff[0] == '.' && buff[1] == '.')
        {
            NextSym();
            return new Token(lineCounter, currColumn, castType(sep), buff);
        }
        else if (f[0] != ':')
        {
            return new Token(lineCounter, currColumn, castType(op), f);
        }
        else return Error("BadChar", currColumn);
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
				else return Error("NoFract", columnCounter);
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
		else return Error("NoHex", columnCounter);
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
			else return Error("NoHex", columnCounter);

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
		else return Error("NoCC", columnCounter);
		if (c >= 0 && c <= 127)
		{
			return new TokenVal<char>(lineCounter, currColumn, castType(character), lexeme, (char)c);
		}
		else return Error("BadCC", columnCounter);
	}
	else if (!end) return Error("BadChar", columnCounter);
}

#endif //LEXOGRAPH_LEXER_H
