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
#include "Token.h"

using namespace std;

#include "Lexer.h"

int main(int argc, char *argv[])
{
	Lexer myLexer("input.txt");
	for (Token* i = myLexer.GetToken(); !i->null(); i = myLexer.GetToken())
	{
		i->PrintToken();
	}

	return 0;
}