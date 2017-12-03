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

	if( 4 < argc || 1 >= argc ) {
		std::cerr << "Erro: Numero incorreto de argumentos passados.\n";
		return EXIT_FAILURE;
	}

	Config::ext = ".o";
	Config::beginEndRequired = 2 < argc;

	Semantico& s = Semantico::GetInstance();
	
	for (int i = 1; i < argc; i++) {
		Config::inputFile = argv[i];
		Config::outputFile = std::string(argv[i]);
		Config::outputFile = Config::outputFile.substr(0, Config::outputFile.find_last_of('.'));

		Scanner::GetInstance().SetFile(Config::inputFile);
		s.Reset();

		std::vector< Expression > code;

		// Passagem zero
		std::fstream codeOutput = std::fstream( Config::outputFile + Config::ext, std::ios_base::out );
		if( !codeOutput ) {
			std::cerr << "Nao foi possivel abrir o arquivo para escrita\n";
			return EXIT_FAILURE;
		}
		if( !s.PassagemZero( code ) ) {
			std::cout << "Codigo possui erros. Arquivo nao gerado.\n";
			codeOutput.close();
			return 0;
		}

		std::vector< int > compiledCode = s.PassagemUnica( code );

		std::string binary = std::to_string( compiledCode[0] );
		for(unsigned int i = 1; i < compiledCode.size(); i++ ) {
			binary += " ";
			binary += std::to_string( compiledCode[i] );
		}

		codeOutput << "H: ";
		codeOutput << Config::outputFile.substr( Config::outputFile.find_last_of( "/\\" )+1 ) << "\n";

		codeOutput << "H: ";
		codeOutput << compiledCode.size() << "\n";

		codeOutput << "H: ";
		codeOutput << s.GetMapaBits() << "\n";

		codeOutput << "TD:";
		auto TD = s.GetTabelaDefinicao();
		for( unsigned int i = 0; i < TD.size(); i++ ) {
			codeOutput << " " << std::get<0>(TD[i]) << " " << std::get<1>(TD[i]);
		}
		codeOutput << "\n";

		codeOutput << "TU:";
		auto TU = s.GetTabelaDefinicao();
		for( unsigned int i = 0; i < TU.size(); i++ ) {
			codeOutput << " " << std::get<0>(TU[i]) << " " << std::get<1>(TU[i]);
		}
		codeOutput << "\n";

		codeOutput << "T: ";
		codeOutput << binary << "\n";
		
		codeOutput.close();
	}
	return 0;
}
