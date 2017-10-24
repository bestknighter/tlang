#include "Semantico.hpp"

#include "Directives.hpp"
#include "Instructions.hpp"

Semantico* Semantico::instance = nullptr;

Semantico::Semantico() : p( Parser::GetInstance() ) {}

Semantico& Semantico::GetInstance() {
	if( nullptr == instance ) {
		instance = new Semantico();
	}
	return *instance;
}

std::vector< Expression > PassagemZero() {
	std::vector< Expression > preprocessedCode;

	Expression e;
	bool eof;
	bool validCode = true;
	do {
		eof = p.GetNextExpression(e);
		if( Directives::Validate( e ) ) {
			// Executar diretiva
		} else if( Instruction::Validate( e ) ) {
			preprocessedCode.push_back( e );
		} else {
			validCode = false;
		}
	} while( !eof );
}
