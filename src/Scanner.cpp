#include "Scanner.hpp"

#include <locale>

Scanner::Scanner()
			: instance( nullptr )
			, filename( "" )
			, currentLine( 0 )
			, nextCh( '' ) {}

Scanner::~Scanner() {}

Scanner& Scanner::GetInstance() {
	if( nullptr == instance ) {
		instance = new Scanner();
	}
	return *instance;
}

void Scanner::SetFile( string file ) {
	if( file.is_open() ) {
		file.close();
	}
	filename = file;
	file = std::fstream( file, std::ios_base::in );
	std::getline( file, line );
	currentLine = 1;
	nextCh = '';
}

int Scanner::GetNextToken( string& nextToken ) {
	do {
		nextToken = SearchNextToken();
	} while( "" == nextToken && '' != nextCh);
	// Validar token
	// Testar se esta funcionando ate chegar em EOF
}

string Scanner::SearchNextToken() {
	if( '\n' == nextCh || ';' == nextCh ) {
		std::getline( file, line );
		currentLine++;
	}
	string nextToken = "";
	std::locale loc;
	while( true ) {
		line.get(nextCh);
		if( ' ' == nextCh || ',' == nextCh || '\t' == nextCh || '\n' == nextCh || ';' == nextCh || '' == nextCh ) {
			break;
		}
		nextToken += std::toupper( nextCh, loc );
	}
	return nextToken;
}
