#include "Scanner.hpp"

#include "Config.hpp"

#include <iostream>
#include <cstdlib>

Scanner* Scanner::instance = nullptr;

Scanner::Scanner()
			: filename( "" )
			, currentLine( 0 )
			, nextCh( 0 ) {}

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
		} while( "" == token && !file.eof());
	}

	nextToken = Token( token, currentLine, currentLineText.size() - (line.size() + token.size()) + (0 == line.size()) );

	// Verifica erros Lexicos
	if( 0 == nextToken.GetValidity() ) {
		std::string errMsg = std::string( "Erro (Lexico) [" ) + Config::inputFile;
		errMsg += ":";
		errMsg += std::to_string( nextToken.GetLine() );
		errMsg += ",";
		errMsg += std::to_string( nextToken.GetColumn() );
		errMsg += std::string( "]: Token mal-formado\n    " );
		errMsg += currentLineText;
		errMsg += "\n    ";
		for( unsigned int i = 0; i < currentLineText.size(); i++ ) {
			char c = ' ';
			if( nextToken.GetColumn()-1 == int(i) ) c = '^';
			else if( nextToken.GetColumn()-1 < int(i) && nextToken.GetText().size() + nextToken.GetColumn()-1 > i ) c = '~';
			errMsg += c;
		}
		errMsg += '\n';
		std::cout << errMsg;
	}
	// End Verifica erros Lexicos

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
