#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "Token.hpp"

#include <string>
#include <fstream>

class Scanner {
	public:
		~Scanner();

		static Scanner& GetInstance();

		void SetFile( std::string filename );
		bool GetNextToken( Token& nextToken );
		int GetCurrentLine();
		std::string GetCurrentLineText();
		std::string GetPreviousLineText();
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
