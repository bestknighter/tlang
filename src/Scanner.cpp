#include "Scanner.hpp"

#include <locale>
#include <iostream>
#include <cstdlib>

Scanner* Scanner::instance = nullptr;

Scanner::Scanner()
			: filename( "" )
			, currentLine( 0 )
			, nextCh( 0 ) {}

Scanner::~Scanner() {}

Scanner& Scanner::GetInstance() {
	if( nullptr == instance ) {
		instance = new Scanner();
	}
	return *instance;
}

void Scanner::SetFile( std::string filename ) {
	if( file.is_open() ) {
		file.close();
	}
	this->filename = filename;
	file = std::fstream( filename, std::ios_base::in );
	if( !file ) {
		std::cerr << "Erro: Arquivo inexistente\n";
		std::exit(EXIT_FAILURE);
	}
	GetNextLine();
	currentLine = 1;
	nextCh = 0;
}

int Scanner::GetCurrentLine() {
	return currentLine;
}

std::string Scanner::GetCurrentLineText() {
	return currentLineText;
}

int Scanner::GetNextToken( Token& nextToken ) {
	std::string token;
	do {
		token = SearchNextToken();
	} while( "" == token && !file.eof());

	nextToken = Token( token, currentLine, currentLineText.size() - (line.size() + token.size()) + (0 == line.size()) );

	if( file.eof() && 0 == line.size() ) return -1;
	return 0;
}

std::string Scanner::SearchNextToken() {
	if( 0 == line.size() || ';' == nextCh ) {
		GetNextLine();
	}

	std::string nextToken = "";
	while( 0 < line.size() ) {
		nextCh = line[0];
		line.erase( 0, 1 );
		if( ' ' == nextCh || ',' == nextCh || '\t' == nextCh || ';' == nextCh ) {
			break;
		}
		nextToken += std::toupper( nextCh );
	}
	
	return nextToken;
}

void Scanner::GetNextLine() {
	std::getline( file, line );
	currentLineText = line;
	currentLine++;
}
