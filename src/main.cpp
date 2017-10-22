#include <string>

#include "Scanner.hpp"

int main( int argc, char** argv ) {

	Scanner& s = Scanner::GetInstance();
	s.SetFile("Arquivos Teste/bin.asm");

	std::string str;
	int code;
	do {
		code = s.GetNextToken(str);
	} while(code != -1);

	return 0;
}
