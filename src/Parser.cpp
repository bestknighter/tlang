#include "Parser.hpp"

#include "Config.hpp"

#include <iostream>

Parser* Parser::instance = nullptr;

Parser::Parser()
			: s( Scanner::GetInstance() )
			, nextToken(new Token()) 
			, lineBeingProcessed( 0 ) {
}

Parser::~Parser() {
	delete nextToken;
}

Parser& Parser::GetInstance() {
	if( nullptr == instance ) {
		instance = new Parser();
	}
	return *instance;
}

bool Parser::GetNextExpression( Expression& exp ) {
	lineBeingProcessed++;
	
	std::vector< Token > tokens;

	if( nextToken->GetLine() == lineBeingProcessed ) {
		tokens.push_back( *nextToken );
	}
	do {
		eof = s.GetNextToken( *nextToken );
		if( nextToken->GetLine() == lineBeingProcessed ) {
			tokens.push_back( *nextToken );
		} else {
			break;
		}
	} while( !eof );

	exp = CreateExpression( tokens );
	return eof;
}

Expression Parser::CreateExpression( std::vector< Token > tokens ) {
	if( tokens.size() == 1 ) { // Com certeza é operando
		if( tokens[0].GetValidity() & Token::Validity::LABEL ) { // Operando Válido
			return Expression( true, "", tokens[0].GetText() );
		} else { // Queria do tipo label mas obteve inválido ou de outro tipo
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
			return Expression( false, "", tokens[0].GetText() );
		}
	}
	return Expression( false, "", "" );
}
