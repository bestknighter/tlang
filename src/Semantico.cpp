#include "Semantico.hpp"

Semantico* Semantico::instance = nullptr;

Semantico::Semantico() : p( Parser::GetInstance() ) {}

Semantico& Semantico::GetInstance() {
	if( nullptr == instance ) {
		instance = new Semantico();
	}
	return *instance;
}

bool Semantico::GetNextExpression(Expression& exp) {
	bool eof = p.GetNextExpression(exp);

	// Procurar erros semanticos
}