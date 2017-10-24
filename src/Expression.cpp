#include "Expression.hpp"

Expression::Expression() : Expression( true, 0, "", "" ) {}

Expression::Expression( bool isValid, unsigned int lineInSource, std::string label, std::string operation
					  , std::string operand1, std::string operand2
					  , int offset1, int offset2 )
				: lineInSource( lineInSource ), isValid( isValid )
				, label( label ), operation( operation )
				, operands( { operand1, operand2 } )
				, offsets( { offset1, offset2 } ) {}

bool Expression::IsValid() {
	return isValid;
}

unsigned int Expression::GetLineInSource() {
	return lineInSource;
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

Expression::operator std::string() {
	std::string inst = "";
	if( 0 < label.size() ) {
		inst += label + ": ";
	}
	inst += operation;
	if( 0 < operands[0].size() ) {
		inst += " ";
		inst += operands[0];
		if( 0 < offsets[0] ) {
			inst += " + ";
			inst += std::to_string( offsets[0] );
		} else if( 0 > offsets[0] ) {
			inst += " - ";
			inst += std::to_string( -offsets[0] );
		}
	}
	if( 0 < operands[1].size() ) {
		inst += ", ";
		inst += operands[1];
		if( 0 < offsets[1] ) {
			inst += " + ";
			inst += std::to_string( offsets[1] );
		} else if( 0 > offsets[1] ) {
			inst += " - ";
			inst += std::to_string( -offsets[1] );
		}
	}
	return inst;
}
