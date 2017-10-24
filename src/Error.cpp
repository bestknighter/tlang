#include "Error.hpp"

#include "Config.hpp"
#include "Scanner.hpp"

#include <iostream>

void Error::Lexico( std::string errorMsg, Token t ) {
	std::string fullLine = Scanner::GetInstance().GetCurrentLineText();
	PrintError( "Lexico", errorMsg, t.GetLine(), t.GetColumn(), fullLine, t.GetColumn(), t.GetColumn()-1 + t.GetText().size() );
}

void Error::Sintatico( std::string errorMsg, Token t ) {
	std::string fullLine = Scanner::GetInstance().GetPreviousLineText();
	PrintError( "Sintatico", errorMsg, t.GetLine(), t.GetColumn(), fullLine, t.GetColumn(), t.GetColumn()-1 + t.GetText().size() );
}

void Error::Semantico() {
	/*
	std::string errMsg = std::string( "Erro (Sintatico) [" ) + Config::inputFile;
	errMsg += ":";
	errMsg += std::to_string( tokens[0].GetLine() );
	errMsg += ",";
	errMsg += std::to_string( tokens[0].GetColumn() );
	errMsg += std::string( "]: Token inesperado. Esperava-se um token do tipo label.\n    " );
	errMsg += s.GetPreviousLineText();
	errMsg += "\n    ";
	for( unsigned int i = 0; i < s.GetPreviousLineText().size(); i++ ) {
		char c = ' ';
		if( tokens[0].GetColumn()-1 == int(i) ) c = '^';
		else if( tokens[0].GetColumn()-1 < int(i) && tokens[0].GetText().size() + tokens[0].GetColumn()-1 > i ) c = '~';
		errMsg += c;
	}
	errMsg += '\n';
	std::cout << errMsg;
	*/
}

void Error::PrintError( std::string type, std::string errorMsg, unsigned int line, unsigned int column, std::string lineOfCode, unsigned int highlightStart, unsigned int highlightEnd ) {
	std::string errMsg = std::string( "Erro (" ) + type + std::string( ") [" );
	errMsg += Config::inputFile + ":";
	errMsg += std::to_string( line ) + ",";
	errMsg += std::to_string( column );
	errMsg += std::string( "]: " ) + errorMsg + ( "\n    " );
	errMsg += lineOfCode;
	errMsg += "\n    ";
	for( unsigned int i = 0; i < lineOfCode.size(); i++ ) {
		char c = ' ';
		if( highlightStart <= i && highlightEnd > i ) c = '~';
		if( column-1 == i ) c = '^';
		errMsg += c;
	}
	errMsg += '\n';
	std::cout << errMsg;
}
