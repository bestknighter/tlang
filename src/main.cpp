#include <string>

#include "Scanner.hpp"
#include "Token.hpp"

int main( int argc, char** argv ) {

	Scanner& s = Scanner::GetInstance();
	s.SetFile("Arquivos Teste/bin.asm");

	Token t;
	int code;
	do {
		code = s.GetNextToken(t);
	} while(code != -1);

	return 0;
}
