#include <iostream>
#include <string>
#include <cstdlib>

#include "Scanner.hpp"
#include "Token.hpp"

int main( int argc, char* argv[] ) {

	if( argc != 4 ) {
		std::cerr << "Erro: Numero incorreto de argumentos passados.\n";
		return EXIT_FAILURE;
	}
	
	// Pegando argumentos do programa
	int numEtapas = 0;
	std::string inputFile( argv[2] );
	std::string outputFile( argv[3] );
	std::string ext;

	{
		std::string arg = argv[1];
		if( "-p" == arg ) {
			numEtapas = 1;
			ext = ".pre";
		} else if( "-m" == arg ) {
			numEtapas = 2;
			ext = ".mcr";
		} else if( "-o" == arg ) {
			numEtapas = 3;
			ext = "";
		} else {
			std::cerr << "Erro: Opcao " << arg << " nao reconhecido.\n";
			return EXIT_FAILURE;
		}
	}
	// End Pegando argumentos do programa

	Scanner& s = Scanner::GetInstance();
	s.SetFile(inputFile);

	Token t;
	int code;
	do {
		code = s.GetNextToken(t);

		// Verifica erros Lexicos
		if( 0 == t.GetValidity() ) {
			std::string errMsg = std::string( "Erro (Lexico) [bin.asm:" );
			errMsg += std::to_string( t.GetLine() );
			errMsg += ",";
			errMsg += std::to_string( t.GetColumn() );
			errMsg += std::string( "]: Token mal-formado\n    " );
			errMsg += s.GetCurrentLineText();
			errMsg += "\n    ";
			for( unsigned int i = 0; i < s.GetCurrentLineText().size(); i++ ) {
				char c = ' ';
				if( t.GetColumn()-1 == int(i) ) c = '^';
				else if( t.GetColumn()-1 < int(i) && t.GetText().size() + t.GetColumn()-1 > i ) c = '~';
				errMsg += c;
			}
			errMsg += '\n';
			std::cout << errMsg;
		}
		// End Verifica erros Lexicos

	} while( code != -1 );

	return 0;
}
