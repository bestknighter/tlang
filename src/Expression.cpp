#include "Expression.hpp"

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
