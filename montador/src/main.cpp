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

	if( 1 >= argc ) {
		std::cerr << "Erro: Numero incorreto de argumentos passados.\n";
		return EXIT_FAILURE;
	}

	Config::ext = ".o";
	Config::beginEndRequired = 2 < argc;

	Semantico& s = Semantico::GetInstance();
	
	// Fazer para todos os arquivos passados
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

		// Passagem unica caso tenha dado tudo certo ate o momento
		std::vector< int > compiledCode = s.PassagemUnica( code );

		// Tem mais de um arquivo? Tem que ter BEGIN e END em todos
		if( 2 < argc ) {
			bool fail = false;
			if( !s.GetTeveBegin() ) {
				std::cerr << "Nao foi possivel encontrar a diretiva BEGIN no arquivo " << Config::inputFile << ".\n";
				fail |= true;
			}
			if( !s.GetTeveEnd() ) {
				std::cerr << "Nao foi possivel encontrar a diretiva END no arquivo " << Config::inputFile << ".\n";
				fail |= true;
			}
			if( fail ) {
				return EXIT_FAILURE;
			}
		}

		// STRING(OPCODE) <- INT(OPCODE)
		std::string binary = std::to_string( compiledCode[0] );
		for(unsigned int i = 1; i < compiledCode.size(); i++ ) {
			binary += " ";
			binary += std::to_string( compiledCode[i] );
		}

		/*************
		 * Cabecalho *
		 *************/
		// Nome do Arquivo
		codeOutput << "H: ";
		codeOutput << Config::outputFile.substr( Config::outputFile.find_last_of( "/\\" )+1 ) << "\n";

		// Tamanho do programa
		codeOutput << "H: ";
		codeOutput << compiledCode.size() << "\n";

		// Mapa de Bits
		codeOutput << "H: ";
		codeOutput << s.GetMapaBits() << "\n";

		// Tabela de Definicao
		codeOutput << "TD:";
		auto TD = s.GetTabelaDefinicao();
		for( unsigned int i = 0; i < TD.size(); i++ ) {
			codeOutput << " " << std::get<0>(TD[i]) << " " << std::get<1>(TD[i]);
		}
		codeOutput << "\n";

		// Tabela de Uso
		codeOutput << "TU:";
		auto TU = s.GetTabelaUso();
		for( unsigned int i = 0; i < TU.size(); i++ ) {
			codeOutput << " " << std::get<0>(TU[i]) << " " << std::get<1>(TU[i]);
		}
		codeOutput << "\n";

		/**********
		 * Codigo *
		 **********/
		codeOutput << "T: ";
		codeOutput << binary << "\n";
		
		codeOutput.close();
	}
	return 0;
}
