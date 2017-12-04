#include "Scanner.hpp"

#include "Config.hpp"
#include "Error.hpp"

#include <cstdlib>
#include <iostream>

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

// Abre um novo arquivo e recomeça o processo de escaneamento
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
		// Retorna ':' e ',' pois também são tokens válidos, apesar de tbm serem separadores
		token = nextCh;
		nextCh = 0;
	} else {
		do {
			token = SearchNextToken(); // Obtem o proximo componente da linha
		} while( token.size() == 0 && (!file.eof() || line.size() > 0)); // Repete enquanto for vazio ou ainda tiver coisa para pegar na linha
	}

	// Gera um token
	nextToken = Token( token, currentLine, currentLineText.size() - (line.size() + token.size()) + (0 == line.size()) );

	// Acabou o arquivo
	if( token.size() == 0 ) return true;
	// Essa condição só acontece se tiver linhas em branco ao final do arquivo.

	if( !nextToken.GetValidity() ) {
		Error::Lexico( "Token mal-formado.", nextToken );
	}

	if( file.eof() && 0 == line.size() && token.size() == 0 ) return true; // Não tem mais o que processar
	return false; // Ainda tem coisa pra processar
}

std::string Scanner::SearchNextToken() {
	if( 0 == line.size() || ';' == nextCh ) {
		// Se for '\0' ou ';', ignore o restante da linha e pege a próxima
		GetNextLine();
	}

	std::string nextToken = "";
	while( 0 < line.size() ) {
		nextCh = line[0];
		line.erase( 0, 1 );
		if( ' ' == nextCh || ',' == nextCh || '\t' == nextCh || ';' == nextCh || ':' == nextCh ) {
			// Chegou num separador, pode retornar
			break;
		}
		// Transforma tudo em caixa alta
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
