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

void NextSym (char &b)
{
	columnCounter++;
	if (!fin.eof()) fin.get(b);
}

int stoi (string s)
{
	int b;
	istringstream str(s);
	str >> b;
	return b;
}

string stor (string s)
{
	float b;
	istringstream str(s);
	str >> b;
	char buff[100];
	if (b != 0) sprintf(buff, "%.4E", b);
	else sprintf(buff, "0");
	return buff;
}

void CountLaC(char &b)
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
				NextSym(b);
			}
			Token indToken = Token (lineCounter, currColumn, ABTypes(lexeme), lexeme);
			indToken.PrintToken();
		}
		if (isdigit(b))
		{
			currColumn = columnCounter;
			lexeme = "";
			bool isReal = false;
			bool printed = false;
			while ((isdigit(b) || b == '.') && !fin.eof() && !printed)
			{
				if (b == '.')
				{
					string buff = ".";
					NextSym(b);
					if (b == '.')
					{
						TokenVal<int> tokInt = TokenVal<int>(lineCounter, currColumn, castType(integer), lexeme, stoi(lexeme));
						tokInt.PrintToken();
						printed = true;
						buff += b;
						Token twoDots = Token(lineCounter, columnCounter - 1, castType(sep), buff);
						twoDots.PrintToken();
						NextSym(b);
					}
					else if (isdigit(b))
					{
						lexeme += buff;
						isReal = true;
					}
					else
					{
						TokenVal<int> tokInt = TokenVal<int>(lineCounter, currColumn, castType(integer), lexeme, stoi(lexeme));
						tokInt.PrintToken();
						printed = true;
						Token dot = Token(lineCounter, columnCounter - 1, castType(sep), buff);
						dot.PrintToken();
					}
				}
				lexeme += b;
				if (!printed) NextSym(b);
			}
			if (!printed)
			{
				if (isReal)
				{
					TokenVal<string> numToken = TokenVal<string>(lineCounter, currColumn, castType(real), lexeme, stor(lexeme));
					numToken.PrintToken();
				}
				else
				{
					TokenVal<int> numToken = TokenVal<int>(lineCounter, currColumn, castType(integer), lexeme, stoi(lexeme));
					numToken.PrintToken();
				}
			}
		}
		CountLaC(b);
	}

	return 0;
}
