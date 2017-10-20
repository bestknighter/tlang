#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <string>
#include <fstream>
#include <cctype>

class Scanner {
	public:
		Scanner& GetInstance();
		~Scanner();

		void SetFile( string file );
		int GetNextToken( string& nextToken );
	private:
		Scanner* instance;
		Scanner();

		string SearchNextToken();

		string filename;
		std::fstream file;
		string line;
		int currentLine;
		char nextCh;
};

#endif // SCANNER_HPP
