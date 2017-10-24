#include "Scanner.hpp"

#include "Config.hpp"
#include "Error.hpp"

#include <iostream>
#include <cstdlib>

Scanner* Scanner::instance = nullptr;

Scanner::Scanner()
			: filename( "" )
			, currentLine( 0 )
			, nextCh( 0 ) {}
			
Scanner::~Scanner() {
	file.close();
}

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

std::string Scanner::GetPreviousLineText() {
	return previousLineText;
}

std::string Scanner::GetCurrentLineText() {
	return currentLineText;
}

bool Scanner::GetNextToken( Token& nextToken ) {
	std::string token;
	if( ':' == nextCh || ',' == nextCh ) {
		token = nextCh;
		nextCh = 0;
	} else {
		do {
			token = SearchNextToken();
		} while( token.size() == 0 && (!file.eof() || line.size() > 0));
	}

	nextToken = Token( token, currentLine, currentLineText.size() - (line.size() + token.size()) + (0 == line.size()) );
	if( token.size() == 0 ) return true;

	if( !nextToken.GetValidity() ) {
		Error::Lexico( "Token mal-formado.", nextToken );
	}

	if( file.eof() && 0 == line.size() ) return true;
	return false;
}

std::string Scanner::SearchNextToken() {
	if( 0 == line.size() || ';' == nextCh ) {
		GetNextLine();
	}

	std::string nextToken = "";
	while( 0 < line.size() ) {
		nextCh = line[0];
		line.erase( 0, 1 );
		if( ' ' == nextCh || ',' == nextCh || '\t' == nextCh || ';' == nextCh || ':' == nextCh ) {
			break;
		}
		nextToken += std::toupper( nextCh );
	}
	
	return nextToken;
}

void Scanner::GetNextLine() {
	std::getline( file, line );
	previousLineText = currentLineText;
	currentLineText = line;
	currentLine++;
}
