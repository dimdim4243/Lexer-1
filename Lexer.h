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
	int lineCounter = 1;
	int columnCounter = 0;
	int currColumn = 0;
	char b;
	string lexeme;
	ifstream fin;
	TokenBuff buffer;
public:
	Lexer() {};
	Lexer(string stream);
	void NextSym(char &b);
	int stoi (string s);
	double stor (string s);
	void CountLaC(char &b);
	Token* GetToken();
};

Lexer::Lexer(string file)
{
	fin.open(file);
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

void Lexer::CountLaC(char &b)
{
	if (b == ' ')
	{
		NextSym(b);
	}
	else if (b == '\t')
	{
		columnCounter += 5 - columnCounter % 4;
		if (!fin.eof()) fin.get(b);
	}
	else if (b == '\n')
	{
		lineCounter++;
		columnCounter = 1;
		if (!fin.eof()) fin.get(b);
	}
}

Token* Lexer::GetToken()
{
	if (!buffer.empty) return buffer.pop();
	if (fin.eof())
	{
		return new Token();
	}
	CountLaC(b);
	lexeme = "";
	if (isalpha(b))
	{
		currColumn = columnCounter;
		lexeme = "";
		wneof(isalnum(b))
		{
			lexeme += b;
			NextSym(b);
		}
		return new Token(lineCounter, currColumn, ABTypes(lexeme), lexeme);
	}
	if(isop(b, 1))
	{
		string buff;
		buff += b;
		NextSym(b);
		if (isop(b, 0))
		{
			buff += b;
			NextSym(b);
			return new Token(lineCounter, columnCounter - 1, castType(op), buff);
		}
		else
		{
			return new Token(lineCounter, columnCounter - 1, castType(op), buff);
		}
	}
	if (isdigit(b))
	{
		bool r = 0;
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
					return new TokenError(lineCounter, columnCounter, "NoFract");
				}
			}
			lexeme += b;
			NextSym(b);
		}
		if (r) return new TokenVal<double>(lineCounter, currColumn, castType(real), lexeme, stor(lexeme));
		else return new TokenVal<int>(lineCounter, currColumn, castType(integer), lexeme, stoi(lexeme));
	}
	return new Token();
}


#endif //LEXOGRAPH_LEXER_H
