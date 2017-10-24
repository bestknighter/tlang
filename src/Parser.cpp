#include "Parser.hpp"

#include "Config.hpp"
#include "Error.hpp"

#include <iostream>
#include <algorithm>
#include <iterator>
#include <array>

Parser* Parser::instance = nullptr;

Parser::Parser()
			: s( Scanner::GetInstance() )
			, nextToken(new Token()) 
			, lineBeingProcessed( 1 ) {
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

	lineBeingProcessed = nextToken->GetLine();

	exp = CreateExpression( tokens );
	return eof;
}

Expression Parser::CreateExpression( std::vector< Token > tokens ) {
	if( tokens.size() == 1 ) { // Operacao sem argumento
		if( tokens[0].GetValidity() & Token::Validity::LABEL ) { // Operacao Válida
			return Expression( true, tokens[0].GetLine(), "", tokens[0].GetText() );
		} else if ( tokens[0].GetValidity() ) { // Queria do tipo label mas obteve de outro tipo
			Error::Sintatico( "Token inesperado. Esperava-se um token do tipo label.", tokens[0] );
			return Expression( false, tokens[0].GetLine(), "", tokens[0].GetText() );
		}
	} else if ( tokens.size() == 2 ) { // Pode ser Label vazio ou operacao com um operando
		if( ":" == tokens[1].GetText() ) { // Label vazio
			if( tokens[0].GetValidity() & Token::Validity::LABEL ) { // Token precisa ser label
				return Expression(true, tokens[0].GetLine(), tokens[0].GetText(), "");
			} else if ( tokens[0].GetValidity() ) {
				Error::Sintatico( "Token inesperado. Esperava-se um token do tipo label.", tokens[0] );
				return Expression( false, tokens[0].GetLine(), tokens[0].GetText(), "" );
			}
		} else { // Operacao com um operando
			if( tokens[0].GetValidity() & Token::Validity::LABEL ) {
				if( tokens[1].GetValidity() & (Token::Validity::LABEL | Token::Validity::NUMBER) ) {
					// Tem quer ser: LABEL LABEL/NUM
					return Expression( true, tokens[0].GetLine(), "", tokens[0].GetText(), tokens[1].GetText() );
				} else if ( tokens[1].GetValidity() ) {
					Error::Sintatico( "Token inesperado. Esperava-se um token do tipo label ou numero.", tokens[1] );
				}
			} else if( tokens[0].GetValidity() ) {
				Error::Sintatico( "Token inesperado. Esperava-se um token do tipo label.", tokens[0] );
			}
			return Expression( false, tokens[0].GetLine(), "", tokens[0].GetText(), tokens[1].GetText() );
		}
	} else {
		std::string label;
		std::string operation;
		bool valid = true;
		int opn = 0;
		std::array< std::string, 2 > operands {"", ""};
		std::array< int, 2 > offsets {0, 0};

		unsigned int i = 0;

		if( ":" == tokens[1].GetText() ) { // Tratando Labels no local certo
			if( tokens[0].GetValidity() & Token::Validity::LABEL ) { // Label Válido
				label = tokens[0].GetText();
				i = 2;
			} else if ( tokens[0].GetValidity() ) { // Queria do tipo label mas obteve de outro tipo
				Error::Sintatico( "Token inesperado. Esperava-se um token do tipo label.", tokens[0] );
				valid = false;
			}
		}
		operation = tokens[i].GetText(); // Detectar erros disso
		for( i++ ; i < tokens.size(); i++ ) {
			if( ":" == tokens[i].GetText() ) { // Opa! Labels ja deveriam ter sido tratados. Isso significa que o label esta num local incorreto
				Error::Sintatico( "Declaracao de label em local nao-esperado!\n\tDeclaracao de labels, quando usados, devem ser a primeira instrucao na linha.", tokens[i-1] );
				valid = false;
				continue;
			}
			if( "," == tokens[i].GetText() ) {
				opn++;
				continue;
			}
			if( tokens[i].GetValidity() & (Token::Validity::LABEL | Token::Validity::NUMBER) ) {
				if( operands[opn].size() == 0 ) {
					operands[opn] = tokens[i].GetText();
				} else {
					opn++;
					operands[opn] = tokens[i].GetText();
					Error::Sintatico( "Caracter ',' esperado antes desse token.", tokens[i] );
					valid = false;
				}
			} else if( i+1 < tokens.size() && (tokens[i].GetValidity() & Token::Validity::ARITHM) && (tokens[i+1].GetValidity() & Token::Validity::NUMBER) ) {
				offsets[opn] = ("+" == tokens[i].GetText() ? 1 : -1) * std::stoi( tokens[i+1].GetText(), 0, 0 );
				if( operands[opn].size() == 0 && 0 != offsets[opn] ) {
					Error::Sintatico( "Inserido offset mas sem label.", tokens[i] );
					valid = false;
				}
				i++;
			} else if( i+1 >= tokens.size() && tokens[i].GetValidity() & Token::Validity::ARITHM ) {
				Error::Sintatico( "Modificador colocado mas sem numero logo apos.", tokens[i] );
				valid = false;
			}
		}
		return Expression( valid, tokens[0].GetLine(), label, operation, operands[0], operands[1], offsets[0], offsets[1] );
	}
	return Expression( false, tokens[0].GetLine(), "", "" );
}
