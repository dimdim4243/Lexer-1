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
	void NextSym();
	bool ishex(char b);
	int static stoi (string s);
	int static shtoi (string s);
	double static stor (string s);
	void CountLaC(char &b);
	Token* GetToken();
	Token* Error(string code);
};

Lexer::Lexer(string file)
{
	error = false;
	fin.open(file);
	lineCounter = 1;
	columnCounter = 0;
	currColumn = 0;
	NextSym();
}

void Lexer::NextSym()
{
	columnCounter++;
	if (!fin.eof()) fin.get(b);
	else b = '~';
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

void Lexer::CountLaC(char &b)
{
	if (b == ' ')
	{
		NextSym();
		CountLaC(b);
	}
	else if (b == '\t')
	{
		columnCounter += 4 - (columnCounter - 1) % 4;
		NextSym();
		CountLaC(b);
	}
	else if (b == '\n')
	{
		lineCounter++;
		columnCounter = 1;
		NextSym();
		CountLaC(b);
	}
}

Token* Lexer::Error(string code)
{
	error = true;
	return new TokenError(lineCounter, columnCounter, code);
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
			NextSym();
		}
		return new Token(lineCounter, currColumn, ABTypes(lexeme), lexeme);
	}
	else if(isop(&b, 1))
	{
		string buff;
		char opr[2] = {b};
		buff += b;
		NextSym();
		opr[1] = b;
		cout << fin.eof();
		if (isop(opr, 0))
		{
			buff += b;
			NextSym();
			return new Token(lineCounter, columnCounter - 1, castType(op), buff);
		}
		else if (opr[0] == '.' && opr[1] == '.')
		{
			buff += b;
			NextSym();
			return new Token(lineCounter, columnCounter -1, castType(sep), buff);
		}
		else
		{
			return new Token(lineCounter, columnCounter - 1, castType(op), buff);
		}
	}
	else if (isdigit(b))
	{
		bool r = 0;
		lexeme = "";
		currColumn = columnCounter;
		wneof(isdigit(b) || (!r && b == '.'))
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
		currColumn = columnCounter;
		NextSym();
		if (ishex(b))
		{
			wneof(ishex(b))
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
				wneof(ishex(b))
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
			wneof(isdigit(b))
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
	else return Error("BadChar");
}

#endif //LEXOGRAPH_LEXER_H
