#include "Lexer.h"

using namespace std;

int main(int argc, char *argv[])
{
	Lexer myLexer("input.txt");
	for (Token* i = myLexer.GetToken(); i != NULL; i = myLexer.GetToken())
	{
		i->PrintToken();
        delete i;
	}

	return 0;
}