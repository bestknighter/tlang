#include "Config.hpp"
#include "Scanner.hpp"
#include "Semantico.hpp"
#include "Expression.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>

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
	Semantico& s = Semantico::GetInstance();

	std::vector< Expression > code;

	// Passagem zero
	std::fstream codeOutput = std::fstream( Config::outputFile + Config::ext, std::ios_base::out );
	if( !codeOutput ) {
		std::cerr << "Nao foi possivel abrir o arquivo para escrita\n";
		return EXIT_FAILURE;
	}
	if( s.PassagemZero( code ) ) {
		if( 3 > Config::numSteps ) {
			for( unsigned int i = 0; i < code.size(); i++ ) {
				codeOutput << std::string( code[i] ) << "\n";
			}
		}
	} else {
		std::cout << "Codigo possui erros. Arquivo nao gerado.\n";
		codeOutput.close();
		return 0;
	}
	if( 3 == Config::numSteps ) {
		codeOutput << s.PassagemUnica( code );
	}
	
	codeOutput.close();
	return 0;
}
