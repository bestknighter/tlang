#include "Parser.hpp"

#include "Config.hpp"
#include "Error.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>

Parser* Parser::instance = nullptr;

Parser::Parser()
			: s( Scanner::GetInstance() )
			, nextToken(new Token()) 
			, lineBeingProcessed( 1 ) {
}

void Parser::Reset() {
	delete nextToken;
	nextToken = new Token();
	lineBeingProcessed = 1;
	eof = false;
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
	do { // Obtem todos os tokens da mesma linha
		eof = s.GetNextToken( *nextToken );
		if( nextToken->GetLine() == lineBeingProcessed ) {
			tokens.push_back( *nextToken );
		} else {
			break;
		}
	} while( !eof );

	lineBeingProcessed = nextToken->GetLine();

	exp = CreateExpression( tokens );
	if( 0 < exp.GetLabel().size() && 0 == exp.GetOperation().size() ) { // Caso seja só um label sozinho
		Expression exp2; // Obtenha a próxima linha
		eof = GetNextExpression( exp2 );
		if( 0 == exp2.GetLabel().size() ) { // Caso a proxima linha nao tenha label
			exp = Expression( exp.IsValid() && exp2.IsValid()
							, exp2.GetLineInSource(), exp.GetLabel()
							, exp2.GetOperation(), exp2.GetOperands()[0]
							, exp2.GetOperands()[1], exp2.GetOffsets()[0]
							, exp2.GetOffsets()[1] );
		} else { // Caso tenha, significa duas labels para a mesma linha de código
			Error::Sintatico( "Proibido declaracao de duas labels por linha de codigo.", tokens[0] );
		}
	}
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
	} else { // Várias combinações possíveis
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
		operation = tokens[i].GetText();
		for( i++ ; i < tokens.size(); i++ ) {
			if( ":" == tokens[i].GetText() ) { // Opa! Labels ja deveriam ter sido tratados. Isso significa que o label está num local incorreto
				Error::Sintatico( "Declaracao de label em local nao esperado!\nDeclaracao de labels, quando usados, devem ser a primeira instrucao na linha.\nIsso pode ser tambem pela declaracao proibida de mais de uma label por instrucao.", tokens[i-1] );
				valid = false;
				break;
			}
			if( "," == tokens[i].GetText() ) { // Passando para segundo argumento
				opn++;
				continue;
			}
			if( tokens[i].GetValidity() & (Token::Validity::LABEL | Token::Validity::NUMBER) ) {
				if( operands[opn].size() == 0 ) {
					operands[opn] = tokens[i].GetText();
				} else {
					opn++;
					operands[opn] = tokens[i].GetText();
					Error::Sintatico( "Caracter ',' esperado antes desse token.", tokens[i] ); // Seria mais um warning, pois nesse caso podemos consertar
					valid = false;
				}
			} else if( i+1 < tokens.size() && (tokens[i].GetValidity() & Token::Validity::ARITHM) && (tokens[i+1].GetValidity() & Token::Validity::NUMBER) ) {
				offsets[opn] = ("+" == tokens[i].GetText() ? 1 : -1) * std::stoi( tokens[i+1].GetText(), 0, 0 );
				if( operands[opn].size() == 0 && 0 != offsets[opn] ) {
					Error::Sintatico( "Inserido offset mas sem ter um label.", tokens[i] );
					valid = false;
				}
				i++;
			} else if( i+1 >= tokens.size() && tokens[i].GetValidity() & Token::Validity::ARITHM ) {
				Error::Sintatico( "Modificador colocado mas sem numero logo apos.", tokens[i] );
				valid = false;
			}
		}
		return Expression( valid, tokens[0].GetLine(), label, operation, operands[0], operands[1], offsets[0], offsets[1] ); // Expressão obtida
	}
	return Expression( false, tokens[0].GetLine(), "", "" ); // Valor simbólico para expressão vazia
}
