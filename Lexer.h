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
	string lexeme;
	ifstream fin;
	TokenBuff buffer;
public:
	Lexer() {};
	Lexer(string stream);
	void NextSym(char &b);
	bool ishex(char b);
	int static stoi (string s);
	int static shtoi (string s);
	double static stor (string s);
	void CountLaC(char &b);
	Token* GetToken();
};

Lexer::Lexer(string file)
{
	error = false;
	fin.open(file);
	lineCounter = 1;
	columnCounter = 0;
	currColumn = 0;
	NextSym(b);
}

void Lexer::NextSym(char &b)
{
	columnCounter++;
	if (!fin.eof()) fin.get(b);
	else b = (char)254;
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
	return (b > 47 && b < 58) || (b > 64 && b < 71) || (b > 96 && b < 103);
}

void Lexer::CountLaC(char &b)
{
	if (b == ' ')
	{
		NextSym(b);
		CountLaC(b);
	}
	else if (b == '\t')
	{
		columnCounter += 4 - (columnCounter - 1) % 4;
		if (!fin.eof()) fin.get(b);
		CountLaC(b);
	}
	else if (b == '\n')
	{
		lineCounter++;
		columnCounter = 1;
		if (!fin.eof()) fin.get(b);
		CountLaC(b);
	}
}

Token* Lexer::GetToken()
{
	if (!buffer.empty) return buffer.pop();
	if (fin.eof() || error) return new Token();
	CountLaC(b);
	lexeme = "";
//	currColumn = columnCounter;
	if (isalpha(b) || b == '_')
	{
		currColumn = columnCounter;
		wneof(isalnum(b) || b == '_')
		{
			lexeme += b;
			NextSym(b);
		}
		return new Token(lineCounter, currColumn, ABTypes(lexeme), lexeme);
	}
	if(isop(&b, 1))
	{
		string buff;
		char opr[2] = {b};
		buff += b;
		NextSym(b);
		opr[1] = b;
		if (isop(opr, 0))
		{
			buff += b;
			NextSym(b);
			return new Token(lineCounter, columnCounter - 1, castType(op), buff);
		}
		else if (opr[0] == '.' && opr[1] == '.')
		{
			buff += b;
			NextSym(b);
			return new Token(lineCounter, columnCounter -1, castType(sep), buff);
		}
		else
		{
			return new Token(lineCounter, columnCounter - 1, castType(op), buff);
		}
	}
	if (isdigit(b))
	{
		bool r = 0;
		lexeme = "";
		currColumn = columnCounter;
		wneof(isdigit(b) || b == '.')
		{
			if (b == '.')
			{
				r = 1;
				NextSym(b);
				if (b == '.')
				{
					Token twoDots(lineCounter, columnCounter - 1, castType(sep), "..");
					buffer.push(&twoDots);
					NextSym(b);
					return new TokenVal<int>(lineCounter, currColumn, castType(integer), lexeme, stoi(lexeme));
				}
				else if (isdigit(b))
				{
					lexeme += '.';
				}
				else
				{
					error = true;
					return new TokenError(lineCounter, columnCounter, "NoFract");
				}
			}
			lexeme += b;
			NextSym(b);
		}
		if (r) return new TokenVal<double>(lineCounter, currColumn, castType(real), lexeme, stor(lexeme));
		else return new TokenVal<int>(lineCounter, currColumn, castType(integer), lexeme, stoi(lexeme));
	}
	if (b == '$')
	{
		string h;
		lexeme += b;
		currColumn = columnCounter;
		NextSym(b);
		if (ishex(b))
		{
			wneof(ishex(b))
			{
				lexeme += b;
				h += b;
				NextSym(b);
			}
			return new TokenVal<int>(lineCounter, currColumn, castType(_hex), lexeme, shtoi(h));
		}
		else
		{
			error = true;
			return new TokenError(lineCounter, columnCounter, "NoHex");
		}
	}
	if (b == '#')
	{
		int c;
		string h;
		lexeme += b;
		NextSym(b);
		if (b == '$')
		{
			lexeme += b;
			NextSym(b);
			if (ishex(b))
			{
				wneof(ishex(b))
				{
					lexeme += b;
					h += b;
					NextSym(b);
				}
				c = shtoi(h);
			}
			else
			{
				error = true;
				return new TokenError(lineCounter, columnCounter, "NoHex");
			}

		}
		else if(isdigit(b))
		{
			wneof(isdigit(b))
			{
				lexeme += b;
				h += b;
				NextSym(b);
			}
			c = stoi(h);
		}
		else
		{
			error = true;
			return new TokenError(lineCounter, columnCounter, "NoCC");
		}
		if (c >= 0 && c <= 127)
		{
			return new TokenVal<char>(lineCounter, currColumn, castType(character), lexeme, (char)c);
		}
		else
		{
			error = true;
			return new TokenError(lineCounter, columnCounter, "BadCC");
		}
	}
	return new Token();
}


#endif //LEXOGRAPH_LEXER_H
