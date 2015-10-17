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
