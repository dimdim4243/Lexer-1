/*
�����  � [0-9]
�����  � [A-Za-z_]
������  � [^'\n]|''
- ������ �� ���� ������� ���������� ���� �������
        �������������_�����������  � {[^}]*}
�������������_�����������  � \(\*[^(\*\))]*\*\)
��������_�����������  � //[^EOF\n]*[EOF\n]
���������� �������  � [ \t\n\r]+
�������������  � {�����}({�����}|{�����})*
�����  � {�����}+
�����������������_�����  � $[0-9A-Fa-f]+
����������  � [Ee][+-]?{�����}
������������_�����  � {�����}\.{�����}
������������_�����  � {�����}(\.{�����})?{����������}
����������_������  � '{������}'
����������_������  � #[{�����}{�����������������_�����}]
������  � '{������}*'
*/
#include <iostream>
#include <sstream>
#include "Token.h"

using namespace std;
ifstream fin("input.txt");

int lineCounter = 1;
int columnCounter = 1;

void nextSym (char &b)
{
	if (b == ' ')
	{
		columnCounter++;
		if (!fin.eof()) fin.get(b);
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
	else
	{
		columnCounter++;
		if (!fin.eof()) fin.get(b);
	}
}

int main()
{

	char b;

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
				nextSym(b);
			}
			Token indToken = Token (lineCounter, currColumn, ABTypes(lexeme), lexeme);
			indToken.PrintToken();
		}
		if (isdigit(b))
		{
			currColumn = columnCounter;
			lexeme = "";
			bool real = false;
			while ((isdigit(b) || b == '.')&& !fin.eof())
			{
				lexeme += b;
				if (b == '.')
				{

				}
				nextSym(b);
			}
			int value;
			istringstream lex(lexeme);
			lex >> value;
			TokenVal<int> intToken = TokenVal<int>(lineCounter, currColumn, castType(integer), lexeme, value);
			intToken.PrintToken();
		}
		nextSym(b);
	}

	return 0;
}
