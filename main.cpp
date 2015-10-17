/*
цифра  Ч [0-9]
буква  Ч [A-Za-z_]
символ  Ч [^'\n]|''
- символ из двух кавычек обозначает одну кавычку
        многострочный_комментарий  Ч {[^}]*}
многострочный_комментарий  Ч \(\*[^(\*\))]*\*\)
строчный_комментарий  Ч //[^EOF\n]*[EOF\n]
пробельные символы  Ч [ \t\n\r]+
идентификатор  Ч {буква}({буква}|{цифра})*
число  Ч {цифра}+
шестнадцатеричное_число  Ч $[0-9A-Fa-f]+
экспонента  Ч [Ee][+-]?{число}
вещественное_число  Ч {число}\.{число}
вещественное_число  Ч {число}(\.{число})?{экспонента}
символьна€_литера  Ч '{символ}'
символьна€_литера  Ч #[{число}{шестнадцатеричное_число}]
строка  Ч '{символ}*'
*/
#include <iostream>
#include <sstream>
#include "Token.h"

using namespace std;

int main()
{
	ifstream fin("input.txt");
	char b;
	int lineCounter = 1;
	int columnCounter = 1;
	int currColumn;
	string lexeme = "";
	fin.get(b);

	while (!fin.eof())
	{
		if (isalpha(b))
		{
			currColumn = columnCounter;
			lexeme = "";
			while (isalnum(b) && !fin.eof())
			{
				lexeme += b;
				columnCounter++;
				fin.get(b);
			}
			Token indToken = Token (lineCounter, currColumn, ABTypes(lexeme), lexeme);
			indToken.PrintToken();
		}
		if (isdigit(b))
		{
			currColumn = columnCounter;
			lexeme = "";
			while (isdigit(b) && !fin.eof())
			{
				lexeme += b;
				columnCounter++;
				fin.get(b);
			}
			int value;
			istringstream lex(lexeme);
			lex >> value;
			TokenVal<int> intToken = TokenVal<int>(lineCounter, currColumn, castType(integer), lexeme, value);
			intToken.PrintToken();
		}
		if (b == ' ')
		{
			columnCounter++;
			if (!fin.eof()) fin.get(b);
		}
		if (b == '\t')
		{
			columnCounter += 5 - columnCounter % 4;
			if (!fin.eof()) fin.get(b);
		}
		if (b == '\n')
		{
			lineCounter++;
			columnCounter = 1;
			if (!fin.eof()) fin.get(b);
		}
	}



	return 0;
}
