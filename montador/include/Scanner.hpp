#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "Token.hpp"

#include <string>
#include <fstream>

/**
 * Class Singleton Scanner
 * É a classe que interfaceia com o arquivo, obtendo caracter a caracter com
 * o objetivo de se extrair os tokens.
 */
class Scanner {
	public:
		~Scanner();

		static Scanner& GetInstance();

		void SetFile( std::string filename ); // Recomeça o processo para o arquivo passado.
		bool GetNextToken( Token& nextToken );
		int GetCurrentLine(); // Obtem a linha que está sendo processada no momento
		std::string GetCurrentLineText(); // Obtem o texto da linha atual
		std::string GetPreviousLineText(); // Obtem o texto da linha anterior
	private:
		static Scanner* instance;
		Scanner();

		std::string SearchNextToken();
		void GetNextLine();

		std::string filename;
		std::fstream file;
		std::string line;
		std::string currentLineText;
		std::string previousLineText;
		int currentLine;
		char nextCh;
};

#endif // SCANNER_HPP
