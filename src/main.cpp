#include <iostream>
#include <string>
#include <cstdlib>

#include "Config.hpp"
#include "Scanner.hpp"
#include "Parser.hpp"

int main( int argc, char* argv[] ) {

	if( argc != 4 ) {
		std::cerr << "Erro: Numero incorreto de argumentos passados.\n";
		return EXIT_FAILURE;
	}
	
	// Pegando argumentos do programa
	{
		Config::inputFile = argv[2];
		Config::outputFile = argv[3];

		std::string arg = argv[1];
		if( "-p" == arg ) {
			Config::numSteps = 1;
			Config::ext = ".pre";
		} else if( "-m" == arg ) {
			Config::numSteps = 2;
			Config::ext = ".mcr";
		} else if( "-o" == arg ) {
			Config::numSteps = 3;
			Config::ext = "";
		} else {
			std::cerr << "Erro: Opcao " << arg << " nao reconhecido.\n";
			return EXIT_FAILURE;
		}
	}
	// End Pegando argumentos do programa

	Scanner::GetInstance().SetFile(Config::inputFile);
	Parser& p = Parser::GetInstance();

	Expression e;
	bool eof;
	do {
		eof = p.GetNextExpression(e);
	} while( !eof );

	return 0;
}
