#include "Instruction.hpp"

#include "Error.hpp"
#include "Token.hpp"

#include <stdexcept>
#include <regex>

const std::map< std::string, std::tuple< unsigned int, unsigned int > > Instruction::instMap = { {"ADD", {1, 2} }, {"SUB", {2, 2} }, {"MULT", {3, 2} }
																							   , {"DIV", {4, 2} }, {"JMP", {5, 2} }, {"JMPN", {6, 2} }
																							   , {"JMPP", {7, 2} }, {"JMPZ", {8, 2} }, {"COPY", {9, 3} }
																							   , {"LOAD", {10, 2} }, {"STORE", {11, 2} }, {"INPUT", {12, 2} }
																							   , {"OUTPUT", {13, 2} }, {"STOP", {14, 1} } };

int Instruction::GetOpcode( std::string key ) {
	try {
		unsigned int opcode = std::get<0>( instMap.at( key ) );
		return int(opcode);
	} catch(std::out_of_range &e) {
		return -1;
	}
}

int Instruction::GetOpsize( std::string key ) {
	try {
		unsigned int opcode = std::get<1>( instMap.at( key ) );
		return int(opcode);
	} catch(std::out_of_range &e) {
		return -1;
	}
}

bool Instruction::Validate( Expression exp ) {
	int opcode = GetOpcode( exp.GetOperation() );
	switch( opcode ) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 10:
		case 11:
		case 12:
		case 13: {
			return ValidateSingleArg( exp );
			break;
		}
		case 9: {
			return ValidateTwoArg( exp );
			break;
		}
		case 14: {
			return ValidateNoArg( exp );
			break;
		}
	}
	return -1 != opcode;
}

bool Instruction::ValidateNoArg( Expression exp ) {
	if( 0 < exp.GetOperands()[0].size() || 0 < exp.GetOperands()[1].size() ) {
		int column = exp.GetLabel().size();
		column += 2*(column>0) + 1;
		Error::Semantico( "Essa instrucao nao aceita argumentos.", exp, column, std::string( exp ).size() );
		exp.Invalidate();
	}
	return true;
}

bool Instruction::ValidateSingleArg( Expression exp ) {
	if( 0 < exp.GetOperands()[1].size() ) {
		int column = exp.GetLabel().size();
		column += 2*(column>0) + 1;
		Error::Semantico( "Essa instrucao nao aceita dois argumentos.", exp, column, std::string( exp ).size() );
		exp.Invalidate();
	}
	if( std::regex_match( exp.GetOperands()[0], Token::Number ) ) {
		int column = exp.GetLabel().size();
		column += 2*(column>0) + 1;
		Error::Semantico( "Essa instrucao nao aceita argumento numerico.", exp, column, std::string( exp ).size() );
		exp.Invalidate();
	}
	return true;
}

bool Instruction::ValidateTwoArg( Expression exp ) {
	if( 0 == exp.GetOperands()[0].size() || 0 == exp.GetOperands()[1].size() ) {
		int column = exp.GetLabel().size();
		column += 2*(column>0) + 1;
		Error::Semantico( "Essa instrucao requisita dois argumentos.", exp, column, std::string( exp ).size() );
		exp.Invalidate();
	}
	if( std::regex_match( exp.GetOperands()[0], Token::Number ) || std::regex_match( exp.GetOperands()[1], Token::Number ) ) {
		int column = exp.GetLabel().size();
		column += 2*(column>0) + 1;
		Error::Semantico( "Essa instrucao nao aceita argumento numerico.", exp, column, std::string( exp ).size() );
		exp.Invalidate();
	}
	return true;
}
