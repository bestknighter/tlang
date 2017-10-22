#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <string>
#include <fstream>

class Scanner {
	public:
		static Scanner& GetInstance();
		~Scanner();

		void SetFile( std::string filename );
		int GetNextToken( std::string& nextToken );
		int GetCurrentLine();
		std::string GetCurrentLineText();
	private:
		static Scanner* instance;
		Scanner();

		std::string SearchNextToken();
		void GetNextLine();

		std::string filename;
		std::fstream file;
		std::string line;
		std::string currentLineText;
		int currentLine;
		char nextCh;
};

#endif // SCANNER_HPP
