#include "Expression.hpp"

/*
Expression::Expression( std::vector< Token > args )
				: isValid(true)
				, label()
				, operation() {
	const unsigned int argsSize = args.size();
	validOP = {false, false};

	if( 0 == argsSize ) {
		cerr << "Error Interno: Expression nao pode receber 0 argumentos.\n";
	} else if( 1 == argsSize ) {
		operation = args[0];
	} else {
	
		if( ":" == args[1].GetText() ) {
			label = args[0];
			validLabel = args[0].GetValidity() & Token::Validity::LABEL;

			operation = args[2];
			validOperation = args[2].GetValidity() & Token::Validity::LABEL;
		} else {
			operation = args[0];
			validOperation = args[0].GetValidity() & Token::Validity::LABEL;
		}
	
		for( unsigned int i = 1 + (":" == args[1]), unsigned int opn = 0; i < argsSize; i++ ) {
			if( "," == args[i] ) {
				opn++;
				continue;
			}
			OPs[opn].push_back( args[i] );
			validOP[opn] &= args[i].GetValidity() & ( Token::Validity::LABEL | Token::Validity::NUMBER | Token::Validity::ARITHM );
		}
	}

}

bool Expression::IsValid() {
	return validLabel & validOperation & validOP[0] & validOP[1];
}
*/

Expression::Expression() : Expression( true, "", "" ) {}

Expression::Expression( bool isValid, std::string label, std::string operation
					  , std::string operand1, std::string operand2
					  , int offset1, int offset2 )
				: isValid( isValid ), label( label )
				, operation( operation )
				, operands( { operand1, operand2 } )
				, offsets( { offset1, offset2 } ) {}

bool Expression::IsValid() {
	return isValid;
}

std::string Expression::GetLabel() {
	return label;
}

std::string Expression::GetOperation() {
	return operation;
}

std::array< std::string, 2 > Expression::GetOperands() {
	return operands;
}

std::array< int, 2 > Expression::GetOffsets() {
	return offsets;
}
