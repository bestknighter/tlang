#include "Expression.hpp"

#include <iostream>

Expression::Expression( std::vector< Token > args )
				: isValid(true)
				, label()
				, operation() {
	const unsigned int argsSize = args.size();
	if( 0 == argsSize ) {
		cerr << "Error Interno: Expression nao pode receber 0 argumentos.\n";
	} else if( 1 == argsSize ) {
		operation = args[0];
	} else {
		if( argsSize > 1 && ":" == args[1].GetText() ) {
			label = args[0];
		}
		for( unsigned int i = 2, unsigned int opn = 0; i < argsSize; i++ ) {
			if( "," == args[i] ) {
				opn++;
				continue;
			}
			OPs[ops] = args[i];
		}
	}

	isValid &= ( "" == label.GetText() || Token::Validity::LABEL == label.GetValidity() );
	isValid &= Token::Validity::LABEL == operation.GetValidity();
}

bool Expression::IsValid() {
	return isValid;
}
